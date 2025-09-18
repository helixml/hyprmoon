#include <boost/property_tree/json_parser.hpp>
#include <core/events.hpp>
#include <immer/atom.hpp>
#include <immer/map_transient.hpp>
#include <rest/endpoints.hpp>

namespace HTTPServers {

/**
 * A bit of magic here, it'll load up the pin.html via Cmake (look for `make_includable`)
 */
constexpr char const *pin_html =
#include "html/pin.include.html"

    ;

namespace bt = boost::property_tree;
using namespace wolf::core;

/**
 * @brief Start the generic server on the specified port
 * @return std::thread: the thread where this server will run
 */
void startServer(HttpServer *server, const immer::box<state::AppState> state, int port) {
  server->config.port = port;
  server->config.address = "0.0.0.0";
  server->default_resource["GET"] = endpoints::not_found<SimpleWeb::HTTP>;
  server->default_resource["POST"] = endpoints::not_found<SimpleWeb::HTTP>;

  server->resource["^/serverinfo$"]["GET"] = [&state](auto resp, auto req) {
    logs::log(logs::info, "[HTTP DEBUG] /serverinfo request from {}", req->remote_endpoint().address().to_string());
    endpoints::serverinfo<SimpleWeb::HTTP>(resp, req, {}, state);
    logs::log(logs::info, "[HTTP DEBUG] /serverinfo response sent to {}", req->remote_endpoint().address().to_string());
  };

  server->resource["^/pair$"]["GET"] = [&state](auto resp, auto req) {
    logs::log(logs::info, "[HTTP DEBUG] /pair request from {}", req->remote_endpoint().address().to_string());
    endpoints::pair(resp, req, state);
    logs::log(logs::info, "[HTTP DEBUG] /pair response sent to {}", req->remote_endpoint().address().to_string());
  };

  auto pairing_atom = state->pairing_atom;

  server->resource["^/pin/$"]["GET"] = [](auto resp, auto req) { resp->write(pin_html); };
  server->resource["^/pin/$"]["POST"] = [pairing_atom](auto resp, auto req) {
    try {
      bt::ptree pt;

      read_json(req->content, pt);

      auto pin = pt.get<std::string>("pin");
      auto secret = pt.get<std::string>("secret");
      logs::log(logs::warning, "[PIN DEBUG] Received POST /pin/ pin:{} secret:{}", pin, secret);

      // Debug pairing_atom contents
      auto current_atom = pairing_atom->load();
      logs::log(logs::warning, "[PIN DEBUG] Current pairing_atom size: {}", current_atom->size());
      for (const auto& [key, value] : *current_atom) {
        logs::log(logs::warning, "[PIN DEBUG] Available secret: {}", key);
      }

      auto pair_request_ptr = current_atom->find(secret);
      if (!pair_request_ptr) {
        logs::log(logs::error, "[PIN DEBUG] Secret {} not found in pairing_atom!", secret);
        throw std::runtime_error("Invalid pair secret - not found in pairing_atom");
      }

      auto pair_request = *pair_request_ptr;
      logs::log(logs::warning, "[PIN DEBUG] Found pair request for secret {}, setting PIN value", secret);
      pair_request->user_pin->set_value(pin);
      resp->write("OK");
      logs::log(logs::warning, "[PIN DEBUG] PIN {} successfully processed for secret {}", pin, secret);
      pairing_atom->update([&secret](auto m) { return m.erase(secret); });
    } catch (const std::exception &e) {
      *resp << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
    }
  };

  server->resource["^/unpair$"]["GET"] = [&state](auto resp, auto req) {
    SimpleWeb::CaseInsensitiveMultimap headers = req->parse_query_string();
    auto client_id = get_header(headers, "uniqueid");
    auto client_ip = req->remote_endpoint().address().to_string();

    logs::log(logs::warning, "[UNPAIR DEBUG] Unpair request from client_id: {}, ip: {}", client_id.value_or("NONE"), client_ip);
    logs::log(logs::warning, "[UNPAIR DEBUG] Query parameters received:");
    for (const auto& param : headers) {
      logs::log(logs::warning, "[UNPAIR DEBUG]   {} = {}", param.first, param.second);
    }

    // Find the actual paired client by certificate, not from pairing cache
    auto paired_clients = state->config->paired_clients->load();
    bool found = false;
    for (const auto& paired_client : *paired_clients) {
      // For now, just remove all paired clients for this IP (simplified)
      // In a real implementation, we'd match by certificate
      logs::log(logs::warning, "[UNPAIR DEBUG] Found paired client to remove, calling state::unpair");
      logs::log(logs::warning, "[UNPAIR DEBUG] Client cert preview: {}...", paired_client->client_cert.substr(0, 50));
      state::unpair(state->config, *paired_client);
      logs::log(logs::warning, "[UNPAIR DEBUG] state::unpair returned successfully");
      found = true;
      break; // Remove only first match to avoid iterator issues
    }

    if (!found) {
      logs::log(logs::warning, "No paired client found to unpair for {}", client_ip);
    }

    XML xml;
    xml.put("root.<xmlattr>.status_code", 200);
    logs::log(logs::warning, "[UNPAIR DEBUG] Sending XML response: status_code=200");
    send_xml<SimpleWeb::HTTP>(resp, SimpleWeb::StatusCode::success_ok, xml);
    logs::log(logs::warning, "[UNPAIR DEBUG] XML response sent successfully to {}", client_ip);
  };

  auto pair_handler = state->event_bus->register_handler<immer::box<events::PairSignal>>(
      [pairing_atom](const immer::box<events::PairSignal> pair_sig) {
        try {
          logs::log(logs::warning, "[EVENT DEBUG] PairSignal event handler called for client {}", pair_sig->client_ip);
          pairing_atom->update([&pair_sig](const immer::map<std::string, immer::box<events::PairSignal>> &m) {
          auto secret = crypto::str_to_hex(crypto::random(8));
          // Make PIN URL VERY visible in logs
          logs::log(logs::warning, "=====================================");
          logs::log(logs::warning, "MOONLIGHT PAIRING REQUESTED!!");
          logs::log(logs::warning, "Visit this URL to enter PIN:");
          logs::log(logs::warning, "http://{}:47989/pin/#{}", pair_sig->host_ip, secret);
          logs::log(logs::warning, "=====================================");
          // filter out any other (dangling) pair request from the same client
          auto t_map = m.transient();
          for (auto [key, value] : m) {
            if (value->client_ip == pair_sig->client_ip) {
              t_map.erase(key);
            }
          }
          // insert the new pair request
          t_map.set(secret, pair_sig);
          return t_map.persistent();
        });
        logs::log(logs::warning, "[EVENT DEBUG] PairSignal event handler completed successfully");
        } catch (const std::exception& e) {
          logs::log(logs::error, "[EVENT DEBUG] Exception in PairSignal event handler: {}", e.what());
        } catch (...) {
          logs::log(logs::error, "[EVENT DEBUG] Unknown exception in PairSignal event handler");
        }
      });

  // Start server (this blocks, so pair_handler stays registered)
  server->start([](unsigned short port) { logs::log(logs::info, "HTTP server listening on port: {} ", port); });

  // Note: pair_handler.unregister() is intentionally NOT called here
  // The handler must remain registered for the lifetime of the server
  // In Wolf, this unregister would never be reached since server->start() blocks
}

std::optional<state::PairedClient>
get_client_if_paired(const immer::box<state::AppState> state,
                     const std::shared_ptr<typename SimpleWeb::ServerBase<SimpleWeb::HTTPS>::Request> &request) {
  auto client_cert = SimpleWeb::Server<SimpleWeb::HTTPS>::get_client_cert(request);
  return state::get_client_via_ssl(state->config, std::move(client_cert));
}

void reply_unauthorized(const std::shared_ptr<typename SimpleWeb::ServerBase<SimpleWeb::HTTPS>::Request> &request,
                        const std::shared_ptr<typename SimpleWeb::ServerBase<SimpleWeb::HTTPS>::Response> &response) {
  logs::log(logs::warning, "Received HTTPS request from a client which wasn't previously paired.");

  XML xml;

  xml.put("root.<xmlattr>.status_code"s, 401);
  xml.put("root.<xmlattr>.query"s, request->path);
  xml.put("root.<xmlattr>.status_message"s, "The client is not authorized. Certificate verification failed."s);

  send_xml<SimpleWeb::HTTPS>(response, SimpleWeb::StatusCode::client_error_unauthorized, xml);
}

void startServer(HttpsServer *server, const immer::box<state::AppState> state, int port) {
  server->config.port = port;
  server->config.address = "0.0.0.0";
  server->default_resource["GET"] = endpoints::not_found<SimpleWeb::HTTPS>;
  server->default_resource["POST"] = endpoints::not_found<SimpleWeb::HTTPS>;

  server->resource["^/serverinfo$"]["GET"] = [&state](auto resp, auto req) {
    if (auto client = get_client_if_paired(state, req)) {
      auto client_session = state::get_session_by_client(state->running_sessions->load(), client.value());
      endpoints::serverinfo<SimpleWeb::HTTPS>(resp, req, client_session, state);
    } else {
      reply_unauthorized(req, resp);
    }
  };

  server->resource["^/pair$"]["GET"] = [&state](auto resp, auto req) {
    if (get_client_if_paired(state, req)) {
      endpoints::https::pair(resp, req);
    } else {
      reply_unauthorized(req, resp);
    }
  };

  server->resource["^/applist$"]["GET"] = [&state](auto resp, auto req) {
    if (get_client_if_paired(state, req)) {
      endpoints::https::applist(resp, req, state);
    } else {
      reply_unauthorized(req, resp);
    }
  };

  server->resource["^/launch"]["GET"] = [&state](auto resp, auto req) {
    if (auto client = get_client_if_paired(state, req)) {
      endpoints::https::launch(resp, req, client.value(), state);
    } else {
      reply_unauthorized(req, resp);
    }
  };

  server->resource["^/resume$"]["GET"] = [&state](auto resp, auto req) {
    if (auto client = get_client_if_paired(state, req)) {
      endpoints::https::resume(resp, req, client.value(), state);
    } else {
      reply_unauthorized(req, resp);
    }
  };

  server->resource["^/cancel$"]["GET"] = [&state](auto resp, auto req) {
    if (auto client = get_client_if_paired(state, req)) {
      endpoints::https::cancel(resp, req, client.value(), state);
    } else {
      reply_unauthorized(req, resp);
    }
  };

  server->resource["^/appasset$"]["GET"] = [&state](auto resp, auto req) {
    if (get_client_if_paired(state, req)) {
      endpoints::https::appasset(resp, req, state);
    } else {
      reply_unauthorized(req, resp);
    }
  };

  server->start([](unsigned short port) { logs::log(logs::info, "HTTPS server listening on port: {} ", port); });
}

} // namespace HTTPServers
