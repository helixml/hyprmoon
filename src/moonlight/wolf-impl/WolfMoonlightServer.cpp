#include "WolfMoonlightServer.hpp"
#include "../../debug/Log.hpp"
#include <sstream>
#include <iomanip>
#include <random>
#include <cstring>

namespace wolf {
namespace core {

// MoonlightState implementation
MoonlightState::MoonlightState(const MoonlightConfig& config) : config_(config) {
}

MoonlightState::~MoonlightState() {
    shutdown();
}

bool MoonlightState::initialize() {
    Debug::log(LOG, "WolfMoonlightState: Initializing with UUID: {}", config_.uuid);
    return true;
}

void MoonlightState::shutdown() {
    std::lock_guard<std::mutex> lock(mutex_);
    active_sessions_.clear();
}

void MoonlightState::addPairedClient(const PairedClient& client) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Remove any existing client with same ID
    paired_clients_.erase(
        std::remove_if(paired_clients_.begin(), paired_clients_.end(),
            [&client](const PairedClient& c) { return c.client_id == client.client_id; }),
        paired_clients_.end());
    
    paired_clients_.push_back(client);
    Debug::log(LOG, "WolfMoonlightState: Paired client {} ({})", client.client_name, client.client_id);
}

void MoonlightState::removePairedClient(const std::string& client_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    paired_clients_.erase(
        std::remove_if(paired_clients_.begin(), paired_clients_.end(),
            [&client_id](const PairedClient& c) { return c.client_id == client_id; }),
        paired_clients_.end());
    
    Debug::log(LOG, "WolfMoonlightState: Unpaired client {}", client_id);
}

std::vector<PairedClient> MoonlightState::getPairedClients() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return paired_clients_;
}

std::shared_ptr<StreamSession> MoonlightState::createSession(const PairedClient& client, const std::string& client_ip) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Generate session ID
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    
    std::stringstream ss;
    ss << std::hex << dis(gen);
    std::string session_id = ss.str();
    
    auto session = std::make_shared<StreamSession>();
    session->session_id = session_id;
    session->client = client;
    session->client_ip = client_ip;
    session->video_port = config_.video_port;
    session->audio_port = config_.audio_port;
    session->start_time = std::chrono::system_clock::now();
    
    active_sessions_[session_id] = session;
    
    Debug::log(LOG, "WolfMoonlightState: Created session {} for client {}", session_id, client.client_name);
    return session;
}

void MoonlightState::destroySession(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    active_sessions_.erase(session_id);
    Debug::log(LOG, "WolfMoonlightState: Destroyed session {}", session_id);
}

std::shared_ptr<StreamSession> MoonlightState::getSession(const std::string& session_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = active_sessions_.find(session_id);
    return (it != active_sessions_.end()) ? it->second : nullptr;
}

// StreamingEngine implementation
StreamingEngine::StreamingEngine(std::shared_ptr<MoonlightState> state) 
    : state_(state), pipeline_(nullptr), app_src_(nullptr), encoder_(nullptr), 
      payloader_(nullptr), sink_(nullptr), running_(false) {
}

StreamingEngine::~StreamingEngine() {
    shutdown();
}

bool StreamingEngine::initialize() {
    Debug::log(LOG, "WolfStreamingEngine: Initializing GStreamer pipeline");
    
    if (!setupGStreamerPipeline()) {
        Debug::log(ERR, "WolfStreamingEngine: Failed to setup GStreamer pipeline");
        return false;
    }
    
    running_ = true;
    gst_thread_ = std::thread(&StreamingEngine::gstreamerThreadMain, this);
    
    Debug::log(LOG, "WolfStreamingEngine: Initialized successfully");
    return true;
}

void StreamingEngine::shutdown() {
    if (running_) {
        running_ = false;
        if (gst_thread_.joinable()) {
            gst_thread_.join();
        }
    }
    
    cleanupGStreamerPipeline();
}

void StreamingEngine::pushFrame(const void* frame_data, size_t size, int width, int height, uint32_t format) {
    if (!running_ || !app_src_) {
        return;
    }
    
    // Create GStreamer buffer from frame data
    GstBuffer* buffer = gst_buffer_new_allocate(nullptr, size, nullptr);
    if (!buffer) {
        Debug::log(ERR, "WolfStreamingEngine: Failed to allocate GstBuffer");
        return;
    }
    
    GstMapInfo map;
    if (gst_buffer_map(buffer, &map, GST_MAP_WRITE)) {
        memcpy(map.data, frame_data, size);
        gst_buffer_unmap(buffer, &map);
        
        // Push buffer to app source
        GstFlowReturn ret = gst_app_src_push_buffer(GST_APP_SRC(app_src_), buffer);
        if (ret != GST_FLOW_OK) {
            Debug::log(WARN, "WolfStreamingEngine: Failed to push buffer: {}", (int)ret);
        }
    } else {
        Debug::log(ERR, "WolfStreamingEngine: Failed to map GstBuffer");
        gst_buffer_unref(buffer);
    }
}

bool StreamingEngine::startStreaming(const std::string& session_id) {
    auto session = state_->getSession(session_id);
    if (!session) {
        Debug::log(ERR, "WolfStreamingEngine: Session {} not found", session_id);
        return false;
    }
    
    session->active = true;
    
    if (pipeline_) {
        gst_element_set_state(pipeline_, GST_STATE_PLAYING);
    }
    
    Debug::log(LOG, "WolfStreamingEngine: Started streaming for session {}", session_id);
    return true;
}

void StreamingEngine::stopStreaming(const std::string& session_id) {
    auto session = state_->getSession(session_id);
    if (session) {
        session->active = false;
    }
    
    if (pipeline_) {
        gst_element_set_state(pipeline_, GST_STATE_PAUSED);
    }
    
    Debug::log(LOG, "WolfStreamingEngine: Stopped streaming for session {}", session_id);
}

bool StreamingEngine::isStreaming(const std::string& session_id) const {
    auto session = state_->getSession(session_id);
    return session && session->active;
}

bool StreamingEngine::setupGStreamerPipeline() {
    std::string pipeline_desc = buildPipelineDescription();
    Debug::log(LOG, "WolfStreamingEngine: Creating pipeline: {}", pipeline_desc);
    
    GError* error = nullptr;
    pipeline_ = gst_parse_launch(pipeline_desc.c_str(), &error);
    
    if (!pipeline_) {
        Debug::log(ERR, "WolfStreamingEngine: Pipeline parse error: {}", error ? error->message : "unknown");
        if (error) g_error_free(error);
        return false;
    }
    
    // Get references to key elements
    app_src_ = gst_bin_get_by_name(GST_BIN(pipeline_), "hyprland_src");
    if (!app_src_) {
        Debug::log(ERR, "WolfStreamingEngine: Failed to get app source element");
        return false;
    }
    
    // Configure app source
    g_object_set(app_src_,
        "caps", gst_caps_from_string("video/x-raw,format=BGRA,width=1920,height=1080,framerate=60/1"),
        "format", GST_FORMAT_TIME,
        "is-live", TRUE,
        nullptr);
    
    return true;
}

void StreamingEngine::cleanupGStreamerPipeline() {
    if (pipeline_) {
        gst_element_set_state(pipeline_, GST_STATE_NULL);
        gst_object_unref(pipeline_);
        pipeline_ = nullptr;
    }
    
    app_src_ = nullptr;
    encoder_ = nullptr;
    payloader_ = nullptr;
    sink_ = nullptr;
}

void StreamingEngine::gstreamerThreadMain() {
    Debug::log(LOG, "WolfStreamingEngine: GStreamer thread started");
    
    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);
    
    if (pipeline_) {
        // Add bus watch
        GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline_));
        gst_bus_add_signal_watch(bus);
        
        // Connect to bus signals
        g_signal_connect(bus, "message::error", G_CALLBACK(+[](GstBus*, GstMessage* msg, gpointer) {
            GError* error;
            gchar* debug_info;
            gst_message_parse_error(msg, &error, &debug_info);
            Debug::log(ERR, "WolfStreamingEngine: GStreamer error: {} ({})", 
                      error->message, debug_info ? debug_info : "no debug info");
            g_error_free(error);
            g_free(debug_info);
        }), nullptr);
        
        g_signal_connect(bus, "message::eos", G_CALLBACK(+[](GstBus*, GstMessage*, gpointer loop) {
            Debug::log(LOG, "WolfStreamingEngine: End of stream");
            g_main_loop_quit((GMainLoop*)loop);
        }), loop);
        
        gst_object_unref(bus);
        
        // Set pipeline to ready state
        gst_element_set_state(pipeline_, GST_STATE_READY);
    }
    
    // Run main loop until shutdown
    while (running_) {
        g_main_context_iteration(nullptr, FALSE);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    g_main_loop_unref(loop);
    Debug::log(LOG, "WolfStreamingEngine: GStreamer thread stopped");
}

std::string StreamingEngine::buildPipelineDescription() const {
    const auto& config = state_->getConfig();
    
    // Build a simple pipeline for now
    std::stringstream ss;
    ss << "appsrc name=hyprland_src ! ";
    ss << "videoconvert ! ";
    
    // Choose encoder based on config
    if (config.video.encoder == "nvenc" || config.video.encoder == "auto") {
        ss << "nvh264enc bitrate=" << config.video.bitrate << " ! ";
    } else if (config.video.encoder == "vaapi") {
        ss << "vaapih264enc bitrate=" << config.video.bitrate << " ! ";
    } else if (config.video.encoder == "qsv") {
        ss << "qsvh264enc bitrate=" << config.video.bitrate << " ! ";
    } else {
        ss << "x264enc bitrate=" << config.video.bitrate << " tune=zerolatency ! ";
    }
    
    ss << "h264parse ! ";
    ss << "rtph264pay ! ";
    ss << "udpsink host=127.0.0.1 port=" << config.video_port;
    
    return ss.str();
}

// ControlServer implementation  
ControlServer::ControlServer(std::shared_ptr<MoonlightState> state)
    : state_(state), server_(nullptr), running_(false) {
}

ControlServer::~ControlServer() {
    shutdown();
}

bool ControlServer::initialize() {
    Debug::log(LOG, "WolfControlServer: Initializing ENet server on port {}", state_->getConfig().control_port);
    
    // Initialize ENet
    if (enet_initialize() != 0) {
        Debug::log(ERR, "WolfControlServer: Failed to initialize ENet");
        return false;
    }
    
    // Create server
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = state_->getConfig().control_port;
    
    server_ = enet_host_create(&address, 32, 2, 0, 0);
    if (!server_) {
        Debug::log(ERR, "WolfControlServer: Failed to create ENet server");
        enet_deinitialize();
        return false;
    }
    
    running_ = true;
    server_thread_ = std::thread(&ControlServer::serverThreadMain, this);
    
    Debug::log(LOG, "WolfControlServer: Initialized successfully");
    return true;
}

void ControlServer::shutdown() {
    if (running_) {
        running_ = false;
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }
    
    if (server_) {
        enet_host_destroy(server_);
        server_ = nullptr;
        enet_deinitialize();
    }
}

void ControlServer::handleMouseInput(int x, int y, int button, bool pressed) {
    // TODO: Forward to connected clients
    Debug::log(TRACE, "WolfControlServer: Mouse input: {},{} button={} pressed={}", x, y, button, pressed);
}

void ControlServer::handleKeyboardInput(int keycode, bool pressed) {
    // TODO: Forward to connected clients  
    Debug::log(TRACE, "WolfControlServer: Keyboard input: key={} pressed={}", keycode, pressed);
}

void ControlServer::handleControllerInput(int controller_id, int button, float value) {
    // TODO: Forward to connected clients
    Debug::log(TRACE, "WolfControlServer: Controller input: id={} button={} value={}", controller_id, button, value);
}

void ControlServer::serverThreadMain() {
    Debug::log(LOG, "WolfControlServer: Server thread started");
    
    while (running_) {
        ENetEvent event;
        while (enet_host_service(server_, &event, 10) > 0) {
            switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                handleClientConnect(event.peer);
                break;
                
            case ENET_EVENT_TYPE_RECEIVE:
                handleClientMessage(event.peer, event.packet->data, event.packet->dataLength);
                enet_packet_destroy(event.packet);
                break;
                
            case ENET_EVENT_TYPE_DISCONNECT:
                handleClientDisconnect(event.peer);
                break;
                
            default:
                break;
            }
        }
    }
    
    Debug::log(LOG, "WolfControlServer: Server thread stopped");
}

void ControlServer::handleClientConnect(ENetPeer* peer) {
    connected_clients_.push_back(peer);
    Debug::log(LOG, "WolfControlServer: Client connected from {}:{}", 
              peer->address.host, peer->address.port);
}

void ControlServer::handleClientDisconnect(ENetPeer* peer) {
    connected_clients_.erase(
        std::remove(connected_clients_.begin(), connected_clients_.end(), peer),
        connected_clients_.end());
    Debug::log(LOG, "WolfControlServer: Client disconnected");
}

void ControlServer::handleClientMessage(ENetPeer* peer, const void* data, size_t size) {
    // TODO: Parse moonlight control protocol messages
    Debug::log(TRACE, "WolfControlServer: Received {} bytes from client", size);
}

// RestServer implementation
RestServer::RestServer(std::shared_ptr<MoonlightState> state)
    : state_(state), running_(false) {
}

RestServer::~RestServer() {
    shutdown();
}

bool RestServer::initialize() {
    Debug::log(LOG, "WolfRestServer: Initializing HTTP server on port {}", state_->getConfig().http_port);
    
    try {
        acceptor_ = std::make_unique<boost::asio::ip::tcp::acceptor>(
            io_context_, 
            boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), state_->getConfig().http_port)
        );
        
        running_ = true;
        server_thread_ = std::thread(&RestServer::serverThreadMain, this);
        
        Debug::log(LOG, "WolfRestServer: Initialized successfully");
        return true;
        
    } catch (const std::exception& e) {
        Debug::log(ERR, "WolfRestServer: Failed to initialize: {}", e.what());
        return false;
    }
}

void RestServer::shutdown() {
    if (running_) {
        running_ = false;
        io_context_.stop();
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
    }
}

void RestServer::handlePairingRequest(const std::string& client_id, const std::string& client_ip) {
    std::lock_guard<std::mutex> lock(pairing_mutex_);
    
    std::string secret = generateSecret();
    
    PairingRequest request;
    request.client_id = client_id;
    request.secret = secret;
    request.client_ip = client_ip;
    request.pin_promise = std::make_shared<std::promise<std::string>>();
    request.timestamp = std::chrono::system_clock::now();
    
    pairing_requests_[secret] = std::move(request);
    
    Debug::log(LOG, "WolfRestServer: Pairing request from {} ({}). Visit http://localhost:{}/pin/#{}", 
              client_id, client_ip, state_->getConfig().http_port, secret);
}

void RestServer::submitPin(const std::string& secret, const std::string& pin) {
    std::lock_guard<std::mutex> lock(pairing_mutex_);
    
    auto it = pairing_requests_.find(secret);
    if (it != pairing_requests_.end()) {
        it->second.pin_promise->set_value(pin);
        Debug::log(LOG, "WolfRestServer: PIN {} submitted for secret {}", pin, secret);
    }
}

void RestServer::serverThreadMain() {
    Debug::log(LOG, "WolfRestServer: Server thread started");
    
    // Simple accept loop
    while (running_) {
        try {
            boost::asio::ip::tcp::socket socket(io_context_);
            acceptor_->accept(socket);
            
            // Handle request in separate thread to avoid blocking
            std::thread([this, socket = std::move(socket)]() mutable {
                handleHttpRequest(std::move(socket));
            }).detach();
            
        } catch (const std::exception& e) {
            if (running_) {
                Debug::log(WARN, "WolfRestServer: Accept error: {}", e.what());
            }
        }
    }
    
    Debug::log(LOG, "WolfRestServer: Server thread stopped");
}

void RestServer::handleHttpRequest(boost::asio::ip::tcp::socket socket) {
    // Basic HTTP request handling - this is very simplified
    try {
        boost::asio::streambuf request;
        boost::asio::read_until(socket, request, "\r\n\r\n");
        
        std::istream request_stream(&request);
        std::string request_line;
        std::getline(request_stream, request_line);
        
        // Parse request line
        std::istringstream iss(request_line);
        std::string method, path, version;
        iss >> method >> path >> version;
        
        Debug::log(TRACE, "WolfRestServer: {} {}", method, path);
        
        std::string response;
        
        if (path == "/serverinfo") {
            response = handleServerInfo();
        } else if (path.starts_with("/pair")) {
            std::map<std::string, std::string> params; // TODO: Parse query parameters
            response = handlePair(params);
        } else if (path.starts_with("/unpair")) {
            std::map<std::string, std::string> params; // TODO: Parse query parameters
            response = handleUnpair(params);
        } else if (path.starts_with("/pin/")) {
            if (method == "GET") {
                response = handlePinPage();
            } else if (method == "POST") {
                // TODO: Read POST body
                response = handlePinSubmit("");
            }
        } else {
            response = "HTTP/1.1 404 Not Found\r\nContent-Length: 9\r\n\r\nNot Found";
        }
        
        boost::asio::write(socket, boost::asio::buffer(response));
        
    } catch (const std::exception& e) {
        Debug::log(WARN, "WolfRestServer: Request handling error: {}", e.what());
    }
}

std::string RestServer::generateSecret() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    
    std::stringstream ss;
    for (int i = 0; i < 16; ++i) {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

std::string RestServer::handleServerInfo() {
    const auto& config = state_->getConfig();
    
    std::stringstream xml;
    xml << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
    xml << "<root status_code=\"200\">";
    xml << "<hostname>" << config.hostname << "</hostname>";
    xml << "<appversion>7.1.431.1</appversion>"; // Mimic GeForce Experience version
    xml << "<GfeVersion>3.27.0.120</GfeVersion>";
    xml << "<uniqueid>" << config.uuid << "</uniqueid>";
    xml << "<MaxLumaPixelsHEVC>1869449984</MaxLumaPixelsHEVC>";
    xml << "<ServerCodecModeSupport>259</ServerCodecModeSupport>";
    xml << "<state>SUNSHINE_STATE_READY</state>";
    xml << "</root>";
    
    std::string body = xml.str();
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: application/xml\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "\r\n";
    response << body;
    
    return response.str();
}

std::string RestServer::handlePair(const std::map<std::string, std::string>& params) {
    // Simplified pairing response
    std::string body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root status_code=\"200\"><paired>1</paired></root>";
    
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: application/xml\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "\r\n";
    response << body;
    
    return response.str();
}

std::string RestServer::handleUnpair(const std::map<std::string, std::string>& params) {
    std::string body = "<?xml version=\"1.0\" encoding=\"utf-8\"?><root status_code=\"200\"></root>";
    
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: application/xml\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "\r\n";
    response << body;
    
    return response.str();
}

std::string RestServer::handlePinPage() {
    std::string html = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Moonlight Pairing</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 50px; }
        .container { max-width: 400px; margin: 0 auto; text-align: center; }
        input[type="text"] { padding: 10px; font-size: 18px; width: 200px; }
        button { padding: 10px 20px; font-size: 16px; margin-left: 10px; }
    </style>
</head>
<body>
    <div class="container">
        <h1>Moonlight Pairing</h1>
        <p>Enter the 4-digit PIN shown on your Moonlight client:</p>
        <form id="pinForm">
            <input type="text" id="pin" maxlength="4" placeholder="PIN" autofocus>
            <button type="submit">Submit</button>
        </form>
    </div>
    <script>
        document.getElementById('pinForm').onsubmit = function(e) {
            e.preventDefault();
            var pin = document.getElementById('pin').value;
            if (pin.length === 4) {
                fetch('/pin/', {
                    method: 'POST',
                    headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({pin: pin, secret: window.location.hash.substring(1)})
                }).then(() => {
                    alert('PIN submitted successfully!');
                });
            }
        };
    </script>
</body>
</html>
)";
    
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/html\r\n";
    response << "Content-Length: " << html.length() << "\r\n";
    response << "\r\n";
    response << html;
    
    return response.str();
}

std::string RestServer::handlePinSubmit(const std::string& body) {
    // TODO: Parse JSON body and extract PIN
    std::string response_body = "OK";
    
    std::stringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/plain\r\n";
    response << "Content-Length: " << response_body.length() << "\r\n";
    response << "\r\n";
    response << response_body;
    
    return response.str();
}

// WolfMoonlightServer implementation
WolfMoonlightServer::WolfMoonlightServer() {
}

WolfMoonlightServer::~WolfMoonlightServer() {
    shutdown();
}

bool WolfMoonlightServer::initialize(const MoonlightConfig& config) {
    Debug::log(LOG, "WolfMoonlightServer: Initializing moonlight server");
    
    if (!initializeGStreamer()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize GStreamer");
        return false;
    }
    
    if (!initializeENet()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize ENet");
        return false;
    }
    
    // Create components
    state_ = std::make_shared<MoonlightState>(config);
    if (!state_->initialize()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize state");
        return false;
    }
    
    streaming_engine_ = std::make_unique<StreamingEngine>(state_);
    if (!streaming_engine_->initialize()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize streaming engine");
        return false;
    }
    
    control_server_ = std::make_unique<ControlServer>(state_);
    if (!control_server_->initialize()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize control server");
        return false;
    }
    
    rest_server_ = std::make_unique<RestServer>(state_);
    if (!rest_server_->initialize()) {
        Debug::log(ERR, "WolfMoonlightServer: Failed to initialize REST server");
        return false;
    }
    
    initialized_ = true;
    Debug::log(LOG, "WolfMoonlightServer: Moonlight server ready on ports HTTP:{}, Control:{}, Video:{}", 
              config.http_port, config.control_port, config.video_port);
    
    return true;
}

void WolfMoonlightServer::shutdown() {
    if (!initialized_) return;
    
    Debug::log(LOG, "WolfMoonlightServer: Shutting down moonlight server");
    
    rest_server_.reset();
    control_server_.reset();
    streaming_engine_.reset();
    state_.reset();
    
    initialized_ = false;
}

void WolfMoonlightServer::onFrameReady(const void* frame_data, size_t size, int width, int height, uint32_t format) {
    if (streaming_engine_) {
        streaming_engine_->pushFrame(frame_data, size, width, height, format);
    }
}

void WolfMoonlightServer::updateConfig(const MoonlightConfig& config) {
    if (state_) {
        state_->updateConfig(config);
    }
}

const MoonlightConfig& WolfMoonlightServer::getConfig() const {
    static MoonlightConfig default_config;
    return state_ ? state_->getConfig() : default_config;
}

bool WolfMoonlightServer::isStreaming() const {
    // Check if any session is active
    if (!state_) return false;
    
    auto clients = state_->getPairedClients();
    return !clients.empty(); // Simplified check
}

int WolfMoonlightServer::getConnectedClientCount() const {
    if (!state_) return 0;
    return state_->getPairedClients().size();
}

std::vector<PairedClient> WolfMoonlightServer::getPairedClients() const {
    if (!state_) return {};
    return state_->getPairedClients();
}

void WolfMoonlightServer::unpairClient(const std::string& client_id) {
    if (state_) {
        state_->removePairedClient(client_id);
    }
}

bool WolfMoonlightServer::initializeGStreamer() {
    if (!gst_is_initialized()) {
        gst_init(nullptr, nullptr);
        Debug::log(LOG, "WolfMoonlightServer: GStreamer initialized");
    }
    return true;
}

bool WolfMoonlightServer::initializeENet() {
    // ENet initialization is handled by ControlServer
    return true;
}

} // namespace core
} // namespace wolf