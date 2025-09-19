#pragma once

#include <control/control.hpp>
#include <protocol/crypto/crypto/crypto.hpp>
#include <curl/curl.h>
#include <curl/easy.h>
#include <core/events.hpp>
#include <filesystem>
#include <functional>
#include <core/utils.hpp>
#include <immer/vector_transient.hpp>
#include <protocol/moonlight/control.hpp>
#include <protocol/moonlight/protocol.hpp>
#include <platforms/hw.hpp>
#include <range/v3/view.hpp>
#include <rest/helpers.hpp>
#include <rest/rest.hpp>
#include <streaming/rtp/udp-ping.hpp>
#include <state/config.hpp>
#include <state/sessions.hpp>
#include <utility>

namespace endpoints {

using namespace control;
using namespace wolf::core;

template <class T> void server_error(const std::shared_ptr<typename SimpleWeb::Server<T>::Response> &response) {
  XML xml;
  xml.put("root.<xmlattr>.status_code", 400);
  send_xml<T>(response, SimpleWeb::StatusCode::client_error_bad_request, xml);
}

template <class T>
void not_found(const std::shared_ptr<typename SimpleWeb::Server<T>::Response> &response,
               const std::shared_ptr<typename SimpleWeb::Server<T>::Request> &request) {
  log_req<T>(request);

  XML xml;
  xml.put("root.<xmlattr>.status_code", 404);
  send_xml<T>(response, SimpleWeb::StatusCode::client_error_not_found, xml);
}

template <class T>
std::string get_host_ip(const std::shared_ptr<typename SimpleWeb::Server<T>::Request> &request,
                        std::shared_ptr<state::AppState> state) {
  return state->host->internal_ip.value_or(request->local_endpoint().address().to_string());
}

template <class T>
void serverinfo(const std::shared_ptr<typename SimpleWeb::Server<T>::Response> &response,
                const std::shared_ptr<typename SimpleWeb::Server<T>::Request> &request,
                std::optional<events::StreamSession> stream_session,
                std::shared_ptr<state::AppState> state) {
  log_req<T>(request);

  SimpleWeb::CaseInsensitiveMultimap headers = request->parse_query_string();

  auto cfg = state->config;
  auto host = state->host;
  bool is_https = std::is_same_v<SimpleWeb::HTTPS, T>;

  bool is_busy = stream_session.has_value();
  int app_id = stream_session.has_value() ? std::stoi(stream_session->app->base.id) : 0;

  auto local_ip = get_host_ip<T>(request, state);

  auto xml = moonlight::serverinfo(is_busy,
                                   app_id,
                                   get_port(state::HTTPS_PORT),
                                   get_port(state::HTTP_PORT),
                                   cfg->uuid,
                                   cfg->hostname,
                                   utils::lazy_value_or(host->mac_address, [&]() { return get_mac_address(local_ip); }),
                                   local_ip,
                                   host->display_modes,
                                   is_https,
                                   cfg->support_hevc,
                                   cfg->support_av1);

  // Log serverinfo response details
  logs::log(logs::warning, "[SERVERINFO DEBUG] Sending serverinfo to {} via {}: is_busy={}, app_id={}, uuid={}, hostname={}",
            request->remote_endpoint().address().to_string(),
            is_https ? "HTTPS" : "HTTP",
            is_busy, app_id, cfg->uuid, cfg->hostname);

  // Log the complete XML response for debugging
  std::stringstream xml_debug;
  boost::property_tree::write_xml(xml_debug, xml);
  logs::log(logs::warning, "[SERVERINFO DEBUG] Complete XML: {}", xml_debug.str());

  send_xml<T>(response, SimpleWeb::StatusCode::success_ok, xml);
}

void remove_pair_session(std::shared_ptr<state::AppState> state, const std::string &cache_key) {
  state->pairing_cache->update([&cache_key](const immer::map<std::string, state::PairCache> &pairing_cache) {
    return pairing_cache.erase(cache_key);
  });
}

XML fail_pair(const std::string &status_msg) {
  logs::log(logs::warning, "Failed pairing: {}", status_msg);

  XML tree;
  tree.put("root.paired", 0);
  tree.put("root.<xmlattr>.status_code", 400);
  tree.put("root.<xmlattr>.status_message", status_msg);

  return tree;
}

struct XMLResult {
  SimpleWeb::StatusCode status;
  XML xml;
};

std::shared_ptr<boost::promise<XMLResult>> pair_phase1(std::shared_ptr<state::AppState> state,
                                                       const std::string &client_ip,
                                                       const std::string &host_ip,
                                                       const std::string &client_cert_str,
                                                       const std::string &salt,
                                                       const std::string &cache_key) {
  auto future_result = std::make_shared<boost::promise<XMLResult>>();

  logs::log(logs::debug, "[PAIR DEBUG] Phase 1 attempt - cache_key: {}, client_ip: {}", cache_key, client_ip);

  auto existing_cache = state->pairing_cache->load()->find(cache_key);
  if (existing_cache) {
    logs::log(logs::warning, "[PAIR DEBUG] Found existing cache entry for {}, last_phase: {}, removing stale entry",
              cache_key, static_cast<int>(existing_cache->last_phase));

    // Remove stale cache entry and allow new pairing attempt
    remove_pair_session(state, cache_key);
    logs::log(logs::info, "[PAIR DEBUG] Cleared stale cache entry for {}, allowing new pairing attempt", cache_key);
  }

  logs::log(logs::info, "[PAIR DEBUG] Starting PIN generation for client {}", client_ip);

  auto future_pin = std::make_shared<boost::promise<std::string>>();
  logs::log(logs::warning, "[PAIR DEBUG] About to fire PIN event for client {}", client_ip);
  state->event_bus->fire_event( // Emit a signal and wait for the promise to be fulfilled
      immer::box<events::PairSignal>(
          events::PairSignal{.client_ip = client_ip, .host_ip = host_ip, .user_pin = future_pin}));

  logs::log(logs::warning, "[PAIR DEBUG] PIN event fired, waiting for user input...");

  future_pin->get_future().then(
      [state, salt, client_cert_str, cache_key, future_result](boost::future<std::string> fut_pin) {
        try {
          auto pin_received = fut_pin.get();
          logs::log(logs::info, "[PAIR DEBUG] PIN received from user: {} for cache_key: {}", pin_received, cache_key);

          logs::log(logs::warning, "[CERT DEBUG] About to call x509::get_cert_pem, server_cert ptr: {}",
                    state->host->server_cert ? "VALID" : "NULL");
          auto server_pem = x509::get_cert_pem(state->host->server_cert);
          logs::log(logs::warning, "[CERT DEBUG] x509::get_cert_pem returned PEM length: {}", server_pem.length());
          logs::log(logs::warning, "[CERT DEBUG] PEM first 100 chars: '{}'",
                    server_pem.length() > 0 ? server_pem.substr(0, std::min(100ul, server_pem.length())) : "EMPTY");

          auto result = moonlight::pair::get_server_cert(pin_received, salt, server_pem);

          logs::log(logs::info, "[PAIR DEBUG] Server cert generated successfully for {}", cache_key);

          auto client_cert_parsed = crypto::hex_to_str(client_cert_str, true);

          state->pairing_cache->update([&](const immer::map<std::string, state::PairCache> &pairing_cache) {
            return pairing_cache.set(cache_key,
                                     {.client_cert = client_cert_parsed,
                                      .aes_key = result.second,
                                      .last_phase = state::PAIR_PHASE::GETSERVERCERT});
          });

          future_result->set_value({SimpleWeb::StatusCode::success_ok, result.first});

          logs::log(logs::info, "[PAIR DEBUG] Phase 1 completed successfully for {}", cache_key);
        } catch (const std::exception& e) {
          logs::log(logs::error, "[PAIR DEBUG] PIN processing failed for {}: {}", cache_key, e.what());
          future_result->set_value({SimpleWeb::StatusCode::client_error_bad_request, fail_pair("PIN processing failed")});
        }
      });

  return future_result;
}

XMLResult pair_phase2(std::shared_ptr<state::AppState> state,
                      state::PairCache &client_cache,
                      const std::string &client_challenge,
                      const std::string &cache_key) {
  if (client_cache.last_phase != state::PAIR_PHASE::GETSERVERCERT) {
    return {SimpleWeb::StatusCode::client_error_bad_request, fail_pair("Out of order pair request (phase 2)")};
  }
  client_cache.last_phase = state::PAIR_PHASE::CLIENTCHALLENGE;

  auto server_cert_signature = x509::get_cert_signature(state->host->server_cert);
  auto [xml, server_secret_pair] =
      moonlight::pair::send_server_challenge(client_cache.aes_key, client_challenge, server_cert_signature);

  auto [server_secret, server_challenge] = server_secret_pair;
  client_cache.server_secret = server_secret;
  client_cache.server_challenge = server_challenge;
  state->pairing_cache->update([&](const immer::map<std::string, state::PairCache> &pairing_cache) {
    return pairing_cache.set(cache_key, client_cache);
  });

  return {SimpleWeb::StatusCode::success_ok, xml};
}

XMLResult pair_phase3(std::shared_ptr<state::AppState> state,
                      state::PairCache &client_cache,
                      const std::string &server_challenge,
                      const std::string &cache_key) {
  if (client_cache.last_phase != state::PAIR_PHASE::CLIENTCHALLENGE) {
    return {SimpleWeb::StatusCode::client_error_bad_request, fail_pair("Out of order pair request (phase 3)")};
  }
  client_cache.last_phase = state::PAIR_PHASE::SERVERCHALLENGERESP;

  auto [xml, client_hash] = moonlight::pair::get_client_hash(client_cache.aes_key,
                                                             client_cache.server_secret.value(),
                                                             server_challenge,
                                                             x509::get_pkey_content(state->host->server_pkey));
  client_cache.client_hash = client_hash;
  state->pairing_cache->update([&](const immer::map<std::string, state::PairCache> &pairing_cache) {
    return pairing_cache.set(cache_key, client_cache);
  });
  return {SimpleWeb::StatusCode::success_ok, xml};
}

XMLResult pair_phase4(state::PairCache &client_cache, const std::string &client_secret) {
  if (client_cache.last_phase != state::PAIR_PHASE::SERVERCHALLENGERESP) {
    return {SimpleWeb::StatusCode::client_error_bad_request, fail_pair("Out of order pair request (phase 4)")};
  }
  client_cache.last_phase = state::PAIR_PHASE::CLIENTPAIRINGSECRET;

  auto client_cert = x509::cert_from_string(client_cache.client_cert);

  if (!client_cert) {
    return {SimpleWeb::StatusCode::client_error_bad_request, fail_pair("Unable to parse client certificate")};
  }

  auto client_sig = x509::get_cert_signature(client_cert);
  auto public_key = x509::get_cert_public_key(client_cert);
  auto xml = moonlight::pair::client_pair(client_cache.aes_key,
                                          client_cache.server_challenge.value(),
                                          client_cache.client_hash.value(),
                                          client_secret,
                                          client_sig,
                                          public_key);

  auto is_paired = xml.get<int>("root.paired") == 1;
  return {is_paired ? SimpleWeb::StatusCode::success_ok : SimpleWeb::StatusCode::client_error_bad_request, xml};
}

void pair(const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTP>::Response> &response,
          const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTP>::Request> &request,
          std::shared_ptr<state::AppState> state) {
  log_req<SimpleWeb::HTTP>(request);

  SimpleWeb::CaseInsensitiveMultimap headers = request->parse_query_string();
  auto salt = get_header(headers, "salt");
  auto client_cert_str = get_header(headers, "clientcert");
  auto client_id = get_header(headers, "uniqueid");
  auto client_ip = request->remote_endpoint().address().to_string();

  logs::log(logs::info, "[PAIR DEBUG] Pairing request from {} - uniqueid: {}, has_salt: {}, has_clientcert: {}",
            client_ip,
            client_id.value_or("NONE"),
            salt.has_value() ? "YES" : "NO",
            client_cert_str.has_value() ? "YES" : "NO");

  if (!client_id) {
    send_xml<SimpleWeb::HTTP>(response,
                              SimpleWeb::StatusCode::client_error_bad_request,
                              fail_pair("Received pair request without uniqueid, stopping."));
    return;
  }

  /* client_id is hardcoded in Moonlight, we add the IP so that different users can pair at the same time */
  auto cache_key = client_id.value() + "@" + client_ip;

  // PHASE 1
  if (client_id && salt && client_cert_str) {
    logs::log(logs::warning, "[PAIR DEBUG] Starting Phase 1 for {}", client_ip);
    auto future_result = pair_phase1(state,
                                     client_ip,
                                     get_host_ip<SimpleWeb::HTTP>(request, state),
                                     client_cert_str.value(),
                                     salt.value(),
                                     cache_key);

    // CRITICAL: Keep promise alive by storing it globally (temporary fix)
    // TODO: Store in proper data structure instead of global variable
    static std::shared_ptr<boost::promise<XMLResult>> persistent_promise;
    persistent_promise = future_result;
    logs::log(logs::warning, "[PAIR DEBUG] Promise stored in persistent variable to prevent destruction");

    future_result->get_future().then([response, client_ip](boost::future<XMLResult> result) {
      auto [status, xml] = result.get();
      logs::log(logs::warning, "[PAIR DEBUG] Phase 1 response for {}: status={}, xml_paired={}",
                client_ip, (int)status, xml.get<int>("root.paired", -1));
      send_xml<SimpleWeb::HTTP>(response, status, xml);
    });
    return;
  }

  auto client_cache_it = state->pairing_cache->load()->find(cache_key);
  if (client_cache_it == nullptr) {
    send_xml<SimpleWeb::HTTP>(
        response,
        SimpleWeb::StatusCode::client_error_bad_request,
        fail_pair(fmt::format("Unable to find {} {} in the pairing cache", client_id.value(), client_ip)));
    return;
  }
  auto client_cache = *client_cache_it;

  // PHASE 2
  auto client_challenge = get_header(headers, "clientchallenge");
  if (client_challenge) {
    auto [status, xml] = pair_phase2(state, client_cache, client_challenge.value(), cache_key);
    logs::log(logs::warning, "[PAIR DEBUG] Phase 2 response: status={}, xml_paired={}",
              (int)status, xml.get<int>("root.paired", -1));
    send_xml<SimpleWeb::HTTP>(response, status, xml);
    if (status != SimpleWeb::StatusCode::success_ok) {
      remove_pair_session(state, cache_key); // security measure, remove the session if the pairing failed
    }
    return;
  }

  // PHASE 3
  auto server_challenge = get_header(headers, "serverchallengeresp");
  if (server_challenge && client_cache.server_secret) {
    auto [status, xml] = pair_phase3(state, client_cache, server_challenge.value(), cache_key);
    logs::log(logs::warning, "[PAIR DEBUG] Phase 3 response: status={}, xml_paired={}",
              (int)status, xml.get<int>("root.paired", -1));
    send_xml<SimpleWeb::HTTP>(response, status, xml);
    if (status != SimpleWeb::StatusCode::success_ok) {
      remove_pair_session(state, cache_key); // security measure, remove the session if the pairing failed
    }
    return;
  }

  // PHASE 4
  auto client_secret = get_header(headers, "clientpairingsecret");
  if (client_secret && client_cache.server_challenge && client_cache.client_hash) {
    auto [status, xml] = pair_phase4(client_cache, client_secret.value());
    logs::log(logs::warning, "[PAIR DEBUG] Phase 4 response: status={}, xml_paired={}",
              (int)status, xml.get<int>("root.paired", -1));

    // Log the complete XML response for debugging
    std::stringstream xml_debug;
    boost::property_tree::write_xml(xml_debug, xml);
    logs::log(logs::warning, "[PAIR DEBUG] Phase 4 complete XML: {}", xml_debug.str());

    send_xml<SimpleWeb::HTTP>(response, status, xml);

    if (status == SimpleWeb::StatusCode::success_ok) {
      logs::log(logs::warning, "[PAIR DEBUG] About to save paired client for {} with enhanced debugging", client_ip);
      state::pair(
          state->config,
          state::PairedClient{.client_cert = client_cache.client_cert,
                              .app_state_folder = std::to_string(std::hash<std::string>{}(client_cache.client_cert))});
      logs::log(logs::warning, "[PAIR DEBUG] Successfully saved paired client for {}", client_ip);
    } else {
      logs::log(logs::warning, "[PAIR DEBUG] Pairing FAILED for {} with status {}", client_ip, (int)status);
    }

    remove_pair_session(state, cache_key); // Either case, this session is done
    return;
  }

  logs::log(logs::warning, "Unable to match pair with any phase, you can retry pairing from Moonlight");
}

namespace https {

/**
 * The check here is implicit, by running over HTTPS we are checking the client certificate
 */
void pair(const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Response> &response,
          const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Request> &request) {
  SimpleWeb::CaseInsensitiveMultimap headers = request->parse_query_string();
  auto phrase = get_header(headers, "phrase");
  // PHASE 5 (over HTTPS)
  if (phrase && phrase.value() == "pairchallenge") {
    XML xml;

    xml.put("root.paired", 1);
    xml.put("root.<xmlattr>.status_code", 200);

    send_xml<SimpleWeb::HTTPS>(response, SimpleWeb::StatusCode::success_ok, xml);
  }
}

void applist(const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Response> &response,
             const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Request> &request,
             std::shared_ptr<state::AppState> state) {
  log_req<SimpleWeb::HTTPS>(request);

  try {
    logs::log(logs::warning, "[APPLIST DEBUG] Starting applist - checking AppState validity");

    if (!state) {
      logs::log(logs::error, "[APPLIST DEBUG] CRITICAL: AppState shared_ptr is null!");
      server_error<SimpleWeb::HTTPS>(response);
      return;
    }

    logs::log(logs::warning, "[APPLIST DEBUG] AppState ptr valid, checking config box");

    // The crash is likely here - accessing state->config (immer::box<Config>)
    logs::log(logs::warning, "[APPLIST DEBUG] About to access state->config...");
    auto config_ptr = &(state->config);
    logs::log(logs::warning, "[APPLIST DEBUG] state->config address: {}", (void*)config_ptr);

    logs::log(logs::warning, "[APPLIST DEBUG] About to dereference config...");
    auto& config = *(state->config);
    logs::log(logs::warning, "[APPLIST DEBUG] Config dereferenced successfully");

    logs::log(logs::warning, "[APPLIST DEBUG] About to access config.apps...");
    auto apps_atom = config.apps;
    if (!apps_atom) {
      logs::log(logs::error, "[APPLIST DEBUG] CRITICAL: config.apps shared_ptr is null!");
      server_error<SimpleWeb::HTTPS>(response);
      return;
    }

    logs::log(logs::warning, "[APPLIST DEBUG] config.apps valid, calling load()...");
    auto apps_loaded = apps_atom->load();
    logs::log(logs::warning, "[APPLIST DEBUG] apps loaded successfully, calling get()...");

    auto base_apps = apps_loaded.get()                                                     //
                     | ranges::views::transform([](const auto &app) { return app->base; }) //
                     | ranges::to<immer::vector<moonlight::App>>();

    logs::log(logs::warning, "[APPLIST DEBUG] Apps transformed successfully, count: {}", base_apps.size());
    auto xml = moonlight::applist(base_apps);

    logs::log(logs::warning, "[APPLIST DEBUG] XML generated, sending response");
    send_xml<SimpleWeb::HTTPS>(response, SimpleWeb::StatusCode::success_ok, xml);
    logs::log(logs::warning, "[APPLIST DEBUG] Response sent successfully");

  } catch (const std::exception& e) {
    logs::log(logs::error, "[APPLIST DEBUG] Exception in applist: {}", e.what());
    server_error<SimpleWeb::HTTPS>(response);
  } catch (...) {
    logs::log(logs::error, "[APPLIST DEBUG] Unknown exception in applist");
    server_error<SimpleWeb::HTTPS>(response);
  }
}

std::optional<std::ifstream> get_file_content(const std::filesystem::path &path) {
  std::ifstream asset_file(path, std::ios::binary);
  if (!asset_file) {
    return {};
  }
  return asset_file;
}

std::optional<std::string> curl_download(const std::string &url) {
  auto curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /* Set custom writer (in order to receive back the response) */
    curl_easy_setopt(curl,
                     CURLOPT_WRITEFUNCTION,
                     static_cast<size_t (*)(char *, size_t, size_t, void *)>(
                         [](char *ptr, size_t size, size_t nmemb, void *read_buf) {
                           *(static_cast<std::string *>(read_buf)) += std::string{ptr, size * nmemb};
                           return size * nmemb;
                         }));
    std::string read_buf;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buf);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    auto res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
      logs::log(logs::warning, "Could not download asset from {}, {}", url, curl_easy_strerror(res));
    } else {
      return read_buf;
    }
  }
  return {};
}

void appasset(const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Response> &response,
              const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Request> &request,
              std::shared_ptr<state::AppState> state) {
  log_req<SimpleWeb::HTTPS>(request);

  SimpleWeb::CaseInsensitiveMultimap headers = request->parse_query_string();
  auto app_id = get_header(headers, "appid");
  if (!app_id) {
    logs::log(logs::warning, "[HTTP] Wrong request, missing app_id");
    server_error<SimpleWeb::HTTPS>(response);
    return;
  }
  auto app = state::get_app_by_id(state->config, app_id.value());
  if (!app || !app.value()->base.icon_png_path) {
    logs::log(logs::trace, "[HTTP] Can't find icon_png_path for app with id: {}", app_id.value());
    server_error<SimpleWeb::HTTPS>(response);
    return;
  }
  auto icon_path = app.value()->base.icon_png_path.value();

  SimpleWeb::CaseInsensitiveMultimap asset_headers;
  asset_headers.emplace("Content-Type", "image/png");
  response->close_connection_after_response = true;

  if (icon_path.starts_with("/")) { // Absolute path
    auto asset_path = std::filesystem::path(icon_path);
    if (auto file_content = get_file_content(asset_path)) {
      response->write(SimpleWeb::StatusCode::success_ok, file_content.value(), asset_headers);
    } else {
      logs::log(logs::warning, "Could not open configured asset: {}", asset_path.string());
      response->write(SimpleWeb::StatusCode::client_error_not_found, "asset not found");
    }
  } else if (icon_path.starts_with("http")) { // URL
    if (auto file_content = curl_download(icon_path)) {
      response->write(SimpleWeb::StatusCode::success_ok, file_content.value(), asset_headers);
    } else {
      response->write(SimpleWeb::StatusCode::client_error_not_found, "asset not found");
    }
  } else { // Assume it's a relative path (legacy setting)
    std::string host_state_folder = utils::get_env("HOST_APPS_STATE_FOLDER", "/etc/wolf");
    auto asset_path = std::filesystem::path(host_state_folder) / icon_path;
    if (auto file_content = get_file_content(asset_path)) {
      response->write(SimpleWeb::StatusCode::success_ok, file_content.value(), asset_headers);
    } else {
      logs::log(logs::warning, "Could not open configured asset: {}", asset_path.string());
      response->write(SimpleWeb::StatusCode::client_error_not_found, "asset not found");
    }
  }
}

auto create_run_session(const SimpleWeb::CaseInsensitiveMultimap &headers,
                        const std::string &client_ip,
                        const state::PairedClient &current_client,
                        std::shared_ptr<state::AppState> state,
                        const events::App &run_app) {
  auto display_mode_str = utils::split(get_header(headers, "mode").value_or("2360x1640x120"), 'x');
  moonlight::DisplayMode display_mode = {std::stoi(display_mode_str[0].data()),
                                         std::stoi(display_mode_str[1].data()),
                                         std::stoi(display_mode_str[2].data()),
                                         state->config->support_hevc,
                                         state->config->support_av1};

  auto surround_info = std::stoi(get_header(headers, "surroundAudioInfo").value_or("196610"));
  int channelCount = surround_info & (0xffff /* last 16 bits */);

  auto base_session = create_stream_session(state,
                                            run_app,
                                            current_client,
                                            display_mode,
                                            channelCount,
                                            get_header(headers, "rikey").value(),
                                            get_header(headers, "rikeyid").value());

  base_session->ip = client_ip;
  return std::move(base_session);
}

std::string get_rtsp_ip_string(const std::string &local_ip, const events::StreamSession &session) {
  auto use_fake_ip = utils::get_env("WOLF_USE_RTSP_FAKE_IP", "TRUE") == "TRUE"s;
  return use_fake_ip ? session.rtsp_fake_ip : local_ip;
}

void launch(const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Response> &response,
            const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Request> &request,
            const state::PairedClient &current_client,
            std::shared_ptr<state::AppState> state) {
  try {
    logs::log(logs::warning, "[LAUNCH DEBUG] Starting launch endpoint");
    log_req<SimpleWeb::HTTPS>(request);

    logs::log(logs::warning, "[LAUNCH DEBUG] Parsing query parameters");
    SimpleWeb::CaseInsensitiveMultimap headers = request->parse_query_string();

    logs::log(logs::warning, "[LAUNCH DEBUG] Getting app_id from headers");
    auto app_id_str = get_header(headers, "appid").value();
    logs::log(logs::warning, "[LAUNCH DEBUG] Requested app_id: {}", app_id_str);

    logs::log(logs::warning, "[LAUNCH DEBUG] Looking up app by ID");
    auto app = state::get_app_by_id(state->config, app_id_str);
    if (!app) {
      logs::log(logs::error, "[LAUNCH DEBUG] App not found for ID: {}", app_id_str);
      server_error<SimpleWeb::HTTPS>(response);
      return;
    }
    logs::log(logs::warning, "[LAUNCH DEBUG] App found: {}", app.value()->base.title);

    logs::log(logs::warning, "[LAUNCH DEBUG] Getting client IP");
    auto client_ip = get_client_ip<SimpleWeb::HTTPS>(request);
    logs::log(logs::warning, "[LAUNCH DEBUG] Client IP: {}", client_ip);

    logs::log(logs::warning, "[LAUNCH DEBUG] Creating run session");
    auto new_session = create_run_session(request->parse_query_string(), client_ip, current_client, state, app.value());
    logs::log(logs::warning, "[LAUNCH DEBUG] Session created with ID: {}", new_session->session_id);

    logs::log(logs::warning, "[LAUNCH DEBUG] Firing stream session event");
    state->event_bus->fire_event(immer::box<events::StreamSession>(*new_session));
    logs::log(logs::warning, "[LAUNCH DEBUG] Event fired successfully");

    logs::log(logs::warning, "[LAUNCH DEBUG] Updating running sessions");
    state->running_sessions->update(
        [&new_session](const immer::vector<events::StreamSession> &ses_v) { return ses_v.push_back(*new_session); });
    logs::log(logs::warning, "[LAUNCH DEBUG] Running sessions updated");

    logs::log(logs::warning, "[LAUNCH DEBUG] Getting RTSP IP");
    auto rtsp_ip = get_rtsp_ip_string(get_host_ip<SimpleWeb::HTTPS>(request, state), *new_session);
    logs::log(logs::warning, "[LAUNCH DEBUG] RTSP IP: {}", rtsp_ip);

    logs::log(logs::warning, "[LAUNCH DEBUG] Generating launch success XML");
    auto xml = moonlight::launch_success(rtsp_ip, std::to_string(get_port(state::RTSP_SETUP_PORT)));
    logs::log(logs::warning, "[LAUNCH DEBUG] XML generated, sending response");

    send_xml<SimpleWeb::HTTPS>(response, SimpleWeb::StatusCode::success_ok, xml);
    logs::log(logs::warning, "[LAUNCH DEBUG] Launch response sent successfully");

  } catch (const std::exception& e) {
    logs::log(logs::error, "[LAUNCH DEBUG] Exception in launch: {}", e.what());
    try {
      server_error<SimpleWeb::HTTPS>(response);
    } catch (...) {
      logs::log(logs::error, "[LAUNCH DEBUG] Failed to send error response");
    }
  } catch (...) {
    logs::log(logs::error, "[LAUNCH DEBUG] Unknown exception in launch");
    try {
      server_error<SimpleWeb::HTTPS>(response);
    } catch (...) {
      logs::log(logs::error, "[LAUNCH DEBUG] Failed to send error response");
    }
  }
}

void resume(const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Response> &response,
            const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Request> &request,
            const state::PairedClient &current_client,
            std::shared_ptr<state::AppState> state) {
  log_req<SimpleWeb::HTTPS>(request);

  auto client_ip = get_client_ip<SimpleWeb::HTTPS>(request);
  auto old_session = state::get_session_by_client(state->running_sessions->load(), current_client);
  if (old_session) {
    auto new_session =
        create_run_session(request->parse_query_string(), client_ip, current_client, state, *old_session->app);
    // Carry over the old session display handle
    new_session->wayland_display = std::move(old_session->wayland_display);
    // Carry over the old session devices, they'll be already plugged into the container
    new_session->mouse = std::move(old_session->mouse);
    new_session->keyboard = std::move(old_session->keyboard);
    new_session->joypads = std::move(old_session->joypads);
    new_session->pen_tablet = std::move(old_session->pen_tablet);
    new_session->touch_screen = std::move(old_session->touch_screen);

    state->running_sessions->update([&old_session, &new_session](const immer::vector<events::StreamSession> ses_v) {
      return state::remove_session(ses_v, old_session.value()).push_back(*new_session);
    });

    auto rtsp_ip = get_rtsp_ip_string(get_host_ip<SimpleWeb::HTTPS>(request, state), *new_session);
    auto xml = moonlight::launch_resume(rtsp_ip, std::to_string(get_port(state::RTSP_SETUP_PORT)));
    send_xml<SimpleWeb::HTTPS>(response, SimpleWeb::StatusCode::success_ok, xml);
  } else {
    logs::log(logs::warning, "[HTTPS] Received resume event from an unregistered session, ip: {}", client_ip);
  }

  server_error<SimpleWeb::HTTPS>(response);
}

void cancel(const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Response> &response,
            const std::shared_ptr<typename SimpleWeb::Server<SimpleWeb::HTTPS>::Request> &request,
            const state::PairedClient &current_client,
            std::shared_ptr<state::AppState> state) {
  logs::log(logs::warning, "[CANCEL DEBUG] Cancel request received");

  try {
    log_req<SimpleWeb::HTTPS>(request);
    logs::log(logs::warning, "[CANCEL DEBUG] Request logged successfully");

    auto client_session = state::get_session_by_client(state->running_sessions->load(), current_client);
    logs::log(logs::warning, "[CANCEL DEBUG] Retrieved client session: {}", client_session ? "FOUND" : "NOT_FOUND");

    if (client_session) {
      logs::log(logs::warning, "[CANCEL DEBUG] Firing stop stream event for session: {}", client_session->session_id);
      state->event_bus->fire_event(
          immer::box<events::StopStreamEvent>(events::StopStreamEvent{.session_id = client_session->session_id}));

      logs::log(logs::warning, "[CANCEL DEBUG] Updating running sessions to remove session");
      state->running_sessions->update([&client_session](const immer::vector<events::StreamSession> &ses_v) {
        return state::remove_session(ses_v, client_session.value());
      });
      logs::log(logs::warning, "[CANCEL DEBUG] Session removed successfully");
    } else {
      logs::log(logs::warning, "[CANCEL DEBUG] Attempting to get client IP for unknown session");
      try {
        auto client_ip = get_client_ip<SimpleWeb::HTTPS>(request);
        logs::log(logs::warning, "[HTTPS] Received cancel event from an unregistered session, ip: {}", client_ip);
      } catch (const std::exception& e) {
        logs::log(logs::error, "[CANCEL DEBUG] Failed to get client IP: {}", e.what());
        logs::log(logs::warning, "[HTTPS] Received cancel event from an unregistered session, ip: UNKNOWN");
      }
    }

    logs::log(logs::warning, "[CANCEL DEBUG] Creating XML response");
    XML xml;
    xml.put("root.<xmlattr>.status_code", 200);
    xml.put("root.cancel", 1);

    logs::log(logs::warning, "[CANCEL DEBUG] Sending XML response");
    try {
      send_xml<SimpleWeb::HTTPS>(response, SimpleWeb::StatusCode::success_ok, xml);
      logs::log(logs::warning, "[CANCEL DEBUG] Cancel response sent, closing connection immediately");

      // CRITICAL: Close connection immediately to prevent async SSL cleanup crashes
      try {
        response->close_connection_after_response = true;
        logs::log(logs::warning, "[CANCEL DEBUG] Connection marked for immediate closure");
      } catch (const std::exception& close_e) {
        logs::log(logs::error, "[CANCEL DEBUG] Failed to mark connection for closure: {}", close_e.what());
      }

      logs::log(logs::warning, "[CANCEL DEBUG] Cancel request completed successfully");
    } catch (const std::exception& e) {
      logs::log(logs::error, "[CANCEL DEBUG] CRITICAL: send_xml failed: {}", e.what());
      // Don't try to send another response - just return
      return;
    } catch (...) {
      logs::log(logs::error, "[CANCEL DEBUG] CRITICAL: send_xml failed with unknown exception");
      return;
    }

  } catch (const std::exception& e) {
    logs::log(logs::error, "[CANCEL DEBUG] Exception in cancel function: {}", e.what());
    try {
      XML error_xml;
      error_xml.put("root.<xmlattr>.status_code", 500);
      error_xml.put("root.cancel", 0);
      logs::log(logs::warning, "[CANCEL DEBUG] Attempting to send error response");
      send_xml<SimpleWeb::HTTPS>(response, SimpleWeb::StatusCode::server_error_internal_server_error, error_xml);
      logs::log(logs::warning, "[CANCEL DEBUG] Error response sent successfully");
    } catch (const std::exception& send_e) {
      logs::log(logs::error, "[CANCEL DEBUG] Failed to send error response: {}", send_e.what());
    } catch (...) {
      logs::log(logs::error, "[CANCEL DEBUG] Failed to send error response - unknown exception");
    }
  } catch (...) {
    logs::log(logs::error, "[CANCEL DEBUG] Unknown exception in cancel function");
    // Don't attempt any response - SSL stream might be corrupted
  }
}

} // namespace https

} // namespace endpoints