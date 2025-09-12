# 🎤🤖 HyprMoon Voice Processing API Examples

## 🗣️ Voice Transcription (Speech → Keyboard)

**Any microphone input** (WebRTC browser, Moonlight client, system mic) automatically converts speech to keyboard input:

```bash
# Start voice transcription
curl -X POST http://localhost:47989/api/voice/start

# Stop voice transcription  
curl -X POST http://localhost:47989/api/voice/stop

# Get voice commands list
curl http://localhost:47989/api/voice/commands
```

**Supported Voice Commands:**
- "new line" / "enter" → Press Enter
- "backspace" → Delete previous character
- "copy" → Ctrl+C
- "paste" → Ctrl+V
- "save" → Ctrl+S
- "select all" → Ctrl+A
- "undo" → Ctrl+Z
- Custom commands (configurable)

## 🤖 Text-to-Speech Robot Voice API

### Speak Text
```bash
# Basic speech
curl -X POST http://localhost:8080/speak \
  -H "Content-Type: application/json" \
  -d '{"text": "Hello from HyprMoon!", "voice": "en+f3"}'

# Different voice
curl -X POST http://localhost:8080/speak \
  -H "Content-Type: application/json" \
  -d '{"text": "Welcome to the future of desktop streaming", "voice": "en+m3"}'

# Robot voice with custom settings
curl -X POST http://localhost:8080/speak \
  -H "Content-Type: application/json" \
  -d '{
    "text": "System alert: WebRTC client connected successfully",
    "voice": "en+f3",
    "rate": 0.8,
    "pitch": 1.2
  }'
```

### Get Available Voices
```bash
curl http://localhost:8080/voices
```

**Response:**
```json
[
  {
    "id": "en+f3",
    "name": "English Female 3",
    "language": "en",
    "gender": "female",
    "description": "Default English female voice"
  },
  {
    "id": "en+m3", 
    "name": "English Male 3",
    "language": "en",
    "gender": "male",
    "description": "Default English male voice"
  }
]
```

### Server Status
```bash
curl http://localhost:8080/status
```

**Response:**
```json
{
  "initialized": true,
  "speaking": false,
  "queue_size": 0,
  "tts_engine": "espeak-ng",
  "default_voice": "en+f3"
}
```

## 🎯 Voice Command Examples

**Dictate text with voice commands:**
```
"Hello world new line this is a test period copy"
```
**Result:** Types "Hello world", presses Enter, types "this is a test.", then Ctrl+C

**Mix speech and commands:**
```
"Dear team comma new line I hope this message finds you well period save"
```
**Result:** Types the text with proper punctuation and saves the document

## 🔧 JavaScript Client Integration

### WebRTC Voice Input
```javascript
// Voice input is automatically captured through WebRTC audio stream
// No additional client code needed - just speak into your microphone!

// Check if voice transcription is active
fetch('/api/voice/status')
  .then(response => response.json())
  .then(data => {
    console.log('Voice transcription active:', data.active);
  });
```

### Robot Voice Output
```javascript
// Make the robot speak
async function speakText(text, voice = 'en+f3') {
  const response = await fetch('http://localhost:8080/speak', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({ text, voice })
  });
  
  if (response.ok) {
    console.log('TTS request sent successfully');
  }
}

// Usage examples
speakText("Welcome to HyprMoon streaming!");
speakText("Connection established", "en+m3");
speakText("Warning: High CPU usage detected", "en+f3");

// Get available voices
async function getVoices() {
  const response = await fetch('http://localhost:8080/voices');
  const voices = await response.json();
  console.log('Available voices:', voices);
  return voices;
}
```

### Voice-Controlled Notifications
```javascript
// Notify about stream events with voice
class VoiceNotifier {
  async notify(message, voice = 'en+f3') {
    await speakText(message, voice);
  }
  
  clientConnected(clientId) {
    this.notify(`Client ${clientId} connected to stream`);
  }
  
  clientDisconnected(clientId) {
    this.notify(`Client ${clientId} disconnected`);
  }
  
  errorOccurred(error) {
    this.notify(`Error occurred: ${error}`, 'en+m3');
  }
}

const voiceNotifier = new VoiceNotifier();
```

## 🚀 Advanced Usage

### Custom Voice Commands
```bash
# Register custom voice command
curl -X POST http://localhost:47989/api/voice/commands \
  -H "Content-Type: application/json" \
  -d '{
    "command": "open terminal",
    "description": "Open a new terminal window",
    "action": "Ctrl+Alt+T"
  }'
```

### Voice-Activated Features
```bash
# Say "connect browser" to create WebRTC offer
# Say "mute voice" to stop transcription
# Say "speak hello world" to use TTS
# Say "save document" to save current file
```

## 🎨 Integration Patterns

### 1. **Dictation Workflow**
- Speak naturally into any microphone
- Text automatically appears as if typed
- Use voice commands for formatting and control

### 2. **Robot Assistant**
- Send HTTP requests to make robot speak
- Perfect for notifications, alerts, status updates
- Customizable voices and speech parameters

### 3. **Hands-Free Control**
- Control desktop entirely by voice
- No need to touch keyboard for basic operations
- Ideal for accessibility and multitasking

### 4. **Developer Workflow**
- Dictate code comments and documentation
- Voice commands for common operations (copy, paste, save)
- TTS for error messages and build status

## 🔧 Configuration

All voice features are configurable via HyprMoon settings:

```toml
[voice]
transcription_enabled = true
tts_enabled = true
whisper_model = "models/ggml-base.en.bin"
tts_engine = "espeak-ng"
default_voice = "en+f3"
tts_port = 8080
voice_commands = true
confidence_threshold = 0.7
```

## 🎯 Use Cases

- **Remote Development**: Dictate code while hands are busy
- **Accessibility**: Full voice control for users with mobility limitations  
- **Presentations**: Voice control during screen sharing
- **Gaming**: Voice commands during gameplay streaming
- **Automation**: Robot voice for system notifications
- **Content Creation**: Hands-free text input while streaming

The voice processing system works seamlessly with both WebRTC (browser) and Moonlight (gaming) clients, providing universal speech-to-text and text-to-speech capabilities across all connected devices! 🚀