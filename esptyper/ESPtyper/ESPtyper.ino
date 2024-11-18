#define BOARD_NAME "Lolin S2 Mini"
#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "index_html.h"

// 配置WiFi
const char *ssid = "ESPtyper";
String password;

// DNS和Web Server
DNSServer dnsServer;
AsyncWebServer server(80);

// USB HID
USBHIDKeyboard Keyboard;

// 按钮状态
bool keyDown = false;

class CaptiveRequestHandler : public AsyncWebHandler
{
  public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
      return true;
    }

    void handleRequest(AsyncWebServerRequest *request) override
    {
      String r_url = request->url();
      Serial.println("url requested: " + r_url);
      Serial.print("Type");
      if ( r_url.startsWith("/type"))
      {
        Serial.print("Text");
        if (request->hasParam("pContent"))
        {
          Serial.print(" received");
          String content = request->getParam("pContent")->value();
          Serial.print(": " + content);
          Serial.println("---<end>---");
          Keyboard.write((const uint8_t *)content.c_str(), content.length());
          Serial.println("Text typing complete");
          request->send(200, "text/plain", "Typed: " + content);
          return;
        }
      } else if (r_url == "/favicon.ico") {
        request->send(404);
        return;
      } else if (r_url.startsWith("/wifi/") ||
                 r_url.startsWith("/mmtls/") ||
                 r_url.startsWith("/c/s/")) {
        request->send(204);  // 返回无内容响应
        return;
      } else
      {
        request->send(200, "text/html", htmlPage);
        return;
      }
    }
};

// 初始化WiFi和Captive Portal
void setupWiFi()
{
  // 生成随机密码
  password = "";
  for (int i = 0; i < 8; i++)
  {
    char randomChar = 'A' + rand() % 26;
    password += randomChar;
  }

  // 配置AP模式
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password.c_str());

  // 配置AP的IP地址
  IPAddress IP = IPAddress(172, 217, 28, 1);
  IPAddress subnet = IPAddress(255, 255, 255, 0);
  WiFi.softAPConfig(IP, IP, subnet);

  // 启动DNS服务器
  dnsServer.start(53, "*", IP);

  Serial.println("WiFi AP setup complete.");
  Serial.println("SSID: " + String(ssid));
  Serial.println("Password: " + password);
  Serial.println("AP IP address: " + IP.toString());
}

// 初始化HID
void setupHID()
{
  pinMode(0, INPUT_PULLUP);
  USB.begin();
  Keyboard.begin();
}

void setup()
{
  Serial.begin(115200);

  // 初始化WiFi和USB HID
  setupWiFi();
  setupHID();
  // 添加Captive Portal处理程序
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);

  // 启动服务器
  server.begin();
}
void loop()
{
  dnsServer.processNextRequest();

  // 检测按钮状态
  if (digitalRead(0) == LOW && !keyDown) {
    keyDown = true;
    delay(50);
    //    hidPrint(password);
    Keyboard.write((const uint8_t*)password.c_str(), password.length());
  }
  else if (digitalRead(0) == HIGH && keyDown) {
    keyDown = false;
    delay(50);
  }
}
