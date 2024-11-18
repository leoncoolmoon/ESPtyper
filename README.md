# ESPtyper
A gadget for copy text from mobile to pc using HID protocol

## 📝 Description
ESPtyper is a versatile tool that allows you to easily transfer text from your mobile device to a computer using USB HID keyboard simulation. It creates a WiFi access point that you can connect to from your mobile device, and then sends the text as keyboard inputs to the connected computer.

## ✨ Features
- 🔒 Secure WiFi access point with random password
- 📱 Mobile-friendly web interface
- ⌨️ USB HID keyboard emulation
- 🔌 Plug-and-play functionality
- 🔄 Real-time text transmission
- 🔐 Built-in password display button

## 🛠 Hardware Requirements
- ESP32-S2 or ESP32-S3 board (Recommended: Lolin S2 Mini)
- USB data cable

## 📦 Dependencies
- ESP32 Arduino Core
- `AsyncTCP`
- `ESPAsyncWebServer`
- `USB.h`
- `USBHIDKeyboard.h`
- `DNSServer.h`

## 📥 Installation
1. Install ESP32 board support in Arduino IDE
2. Install required libraries from the Arduino Library Manager
3. Clone this repository or download the source code
4. Open `ESPtyper.ino` in Arduino IDE
5. Select the correct board and port
6. Upload the sketch to your ESP32 board (wemos s2 mini in my case)

## 🚀 Usage
1. Connect the ESP32 board to your computer via USB
2. The device will create a WiFi access point named "ESPtyper"
3. Connect to the WiFi network using your mobile device
   - SSID: "ESPtyper"
   - Password: Check serial monitor or press the onboard button on gpio 0
4. Your mobile device should automatically open the web interface
   - If not, navigate to any http site or http://172.217.28.1
5. Type or paste text in the web interface
6. Click "type" to type the text on your computer

## 🔧 Configuration
- Default WiFi SSID: "ESPtyper"
- when you connected a captive portal will popout for use
- you can also use http in the web browser to access the typing interface 


## 📝 Notes
- The device acts as a USB HID keyboard, so no drivers are required
- Text is sent character by character with a small delay to ensure reliability
- The web interface works with most modern browsers
- The random password is regenerated each time the device boots

## 🔒 Security
- A random 8-character password is generated on first boot
- The password consists of uppercase letters only
- The password can be displayed by pressing the onboard button
- The WiFi network is secured with WPA2

## 🤝 Contributing
Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests

## 📄 License
This project is released under the MIT License.

## ⚠️ Disclaimer
This device simulates a USB keyboard. Please be cautious when using it in security-sensitive environments.
