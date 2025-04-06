#define VER 1.5
#define BOARD_NAME "Lolin S2 Mini"
#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
#include "index_html.h"
#include "OTA.h"
#define TEST
#define LED 15 // LED引脚

// 配置WiFi
const char *ssid = "ESPtyper";
String password;

// DNS和Web Server
DNSServer dnsServer;
AsyncWebServer server(80);

// USB HID
USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

// 按钮状态
bool keyDown = false;

class CaptiveRequestHandler : public AsyncWebHandler
{
  public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request)
    {
      return true;
    }

    void handleRequest(AsyncWebServerRequest *request) override
    {
      String r_url = request->url();
      Serial.println("url requested: " + r_url);

      if (r_url.startsWith("/type"))
      {
        Serial.print("Text");
        if (request->hasParam("pContent"))
        {
          String content = request->getParam("pContent")->value();
          Serial.print(": " + content);
          Serial.println("---<end>---");
          Keyboard.write((const uint8_t *)content.c_str(), content.length());
          request->send(200, "text/plain", "Typed: " + content);
          return;
        }
        else if (request->hasParam("pKey"))
        {
          // 处理单个按键输入
          String key = request->getParam("pKey")->value();
          Serial.println("KeyCode: " + key);
          String state = request->hasParam("pReleased") ? request->getParam("pReleased")->value() : "";
          Serial.println("Keystate: " + state);
          handleKeyPress(key.toInt(), state);
          request->send(200, "text/plain", "Key sent: " + key + "\n Key state: " + state);
          return;
        }
        else if (request->hasParam("pAction"))
        {
          int action = (request->getParam("pAction")->value()).toInt();
          Serial.println("Action: " + String(action));
          /*
            'Left': 0x01,//pA 0 = button up, 1 = button down, 2 = button click
            'Right': 0x02,//pA 0 = button up, 1 = button down
            'Forward': 0x10,//pA 2 = button click
            'Backward': 0x08,//pA 2 = button click
            'Middle': 0x04,//pA 2 = button click
            'Scroll': 0x0A,//need further action in MCU, pX and pY
            'Touch': 0x0B//need further action in MCU, pX and pY
          */
          if (action < 17) {
            if (request->hasParam("pA"))
            {
              int parameter = (request->getParam("pA")->value()).toInt();//pA 0 = button up, 1 = button down, 2 = button click
              Serial.println("p: " + String(parameter));
              switch (parameter) {
                case 0:
                  if (Mouse.isPressed(action)) {
                    Serial.println("release key " + String(action));
                    Mouse.release(action);
                  }
                  break;
                case 1:
                  if (!Mouse.isPressed(action)) {
                    Serial.println("press key " + String(action));
                    Mouse.press(action);
                  }
                  break;
                case 2:
                  Serial.println("click key " + String(action));
                  Mouse.click(action);
                  break;
              }
            } else {
              Mouse.click(action);
            }
          } else if (request->hasParam("pX") && request->hasParam("pY")) {
            int parameterX = (request->getParam("pX")->value()).toInt();
            int parameterY = (request->getParam("pY")->value()).toInt();
            //Serial.println("pX: " + String(parameterX));
            //Serial.println("pY: " + String(parameterY));
            if (action == 27) { //move mouse
              Mouse.move(parameterX, parameterY, 0, 0);
            } else if (action == 26) { //scroll page
              Mouse.move(0, 0, parameterX, parameterY);
            } else {
              request->send(200, "text/plain", "Error: This mouse action not support yet on MCU");
              return;//not support yet on MCU
            }
          } else {
            request->send(500, "text/plain", "Error: Invalid premeter");
            return;
          }
          request->send(200, "text/plain", "Mouse action done: " + String(action));
          return;
        }
        request->send(500, "text/plain", "Error: Invalid request");
        return;
      }
      else if (r_url == "/favicon.ico")
      {
        request->send(404);
      }
      else if (r_url.startsWith("/wifi/") || r_url.startsWith("/mmtls/") || r_url.startsWith("/c/s/"))
      {
        request->send(204);
      }
      else
      {
        request->send(200, "text/html", htmlPage);
      }
    }
    bool ctrlPressed = false;
    bool shiftPressed = false;
    bool altPressed = false;

  private:
    void handleKeyPress(const int keyCode, const String &keyState)
    {
      // 处理功能键按下
      if (keyState == "false")
      {
        switch (keyCode)
        {
          case 128:
            ctrlPressed = true;
            return;
          case 129:
            shiftPressed = true;
            return;
          case 130:
            altPressed = true;
            return;
        }
      }
      // 处理功能键释放
      else if (keyState == "true")
      {
        switch (keyCode)
        {
          case 128:
            ctrlPressed = false;
            return;
          case 129:
            shiftPressed = false;
            return;
          case 130:
            altPressed = false;
            return;
        }
      }
      // 处理普通键
      else
      {
        Serial.printf("CTL %s, Shift %s, ALT %s\n",
                      "false\0true" + 6 * ctrlPressed,
                      "false\0true" + 6 * shiftPressed,
                      "false\0true" + 6 * altPressed);
        // 如果有功能键被按下，确保它们已经生效
        if (ctrlPressed)
        {
          Keyboard.press(KEY_LEFT_CTRL);
        }
        if (shiftPressed)
        {
          Keyboard.press(KEY_LEFT_SHIFT);
        }
        if (altPressed)
        {
          Keyboard.press(KEY_LEFT_ALT);
        }

        // 按下普通键
        Keyboard.press(keyCode);
        delay(100);

        // 释放所有按键
        Keyboard.releaseAll();

        // 重置功能键状态
        ctrlPressed = false;
        shiftPressed = false;
        altPressed = false;
      }
    }
    // LED闪烁函数
    void blinkLED()
    {
      for (int i = 0; i < 4; i++)
      {
        digitalWrite(LED, HIGH);
        delay(200);
        digitalWrite(LED, LOW);
        delay(200);
      }
    }
};

void setupWiFi()
{
  password = "";
  for (int i = 0; i < 8; i++)
  {
    password += (char)('A' + random(26));
  }
#ifdef TEST
  password = "ABCDEFGH";
#endif
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password.c_str());
  IPAddress IP(172, 217, 28, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(IP, IP, subnet);
  dnsServer.start(53, "*", IP);
  Serial.println("WiFi AP setup complete.");
  Serial.println("SSID: " + String(ssid));
  Serial.println("Password: " + password);
  Serial.println("AP IP address: " + IP.toString());
}

void setup()
{
  Serial.begin(115200);
  pinMode(0, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  setupWiFi();
  Serial.println("USB enabled");
  Keyboard.begin();
  Mouse.begin();
  USB.begin();
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);
  server.begin();
  setupOTA();
}

void loop()
{
  dnsServer.processNextRequest();
  loopOTA();
  if (digitalRead(0) == LOW && !keyDown)
  {
    keyDown = true;
    delay(50);
    Keyboard.write((const uint8_t *)password.c_str(), password.length());
  }
  else if (digitalRead(0) == HIGH && keyDown)
  {
    keyDown = false;
    delay(50);
  }
}
