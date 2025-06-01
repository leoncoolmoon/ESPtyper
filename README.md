# ESPtyper
A gadget for copy text from mobile to pc using HID protocol

## 📝 Description
ESPtyper is a versatile tool that allows you to easily transfer text from your mobile device to a computer using USB HID keyboard simulation. It creates a WiFi access point that you can connect to from your mobile device, and then sends the text as keyboard inputs to the connected computer.
The version 1.6 add ocr function (works when not in captive portal version) 
(You need to modify the borad.txt and add bigOTA.csv at\partitions to make both OTA and ocr working,otherwise if you don't need OTA, you can just choose a big program to make it work, or modify the code and take the ocrad.js into spiffs or fatfs and rewrite a bit code)

### Add big program and bigOTA
You need to add bigOTA.csv to your  "C:\Users\$username\AppData\Local\Arduino15\packages\esp32\hardware\esp32\$version\tools\partitions\"
add following into your broad.txt at "C:\Users\$username\AppData\Local\Arduino15\packages\esp32\hardware\esp32\$version" to make it work.
'''
lolin_s2_mini.menu.PartitionScheme.LargeApp=Default 4MB with OTA(1.9MB APP/1.9MB OTA)
lolin_s2_mini.menu.PartitionScheme.LargeApp.build.partitions=bigOTA
lolin_s2_mini.menu.PartitionScheme.LargeApp.upload.maximum_size=2031616
'''

## ✨ Features
- 🔒 Secure WiFi access point with random password(optional)
- 📱 Mobile-friendly web interface
- ⌨️ USB HID keyboard and mouse emulation
- 🔌 Plug-and-play functionality
- 🔄 Real-time text transmission
- 🔐 Built-in password display button
- build-in OCR function
- build-in OTA function

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

## ⚠️ Disclaimer
This device simulates a USB keyboard. Please be cautious when using it in security-sensitive environments.
