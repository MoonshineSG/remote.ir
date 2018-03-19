/*
Based on IRremoteESP8266: IRServer demo
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>

const char* ssid = "";
const char* password = "";
MDNSResponder mdns;

ESP8266WebServer server(80);

#define IR_LED 4  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(IR_LED);  // Set the GPIO to be used to sending the message.

void handleIr() {
  uint32_t code = 0;
  std::string code_hex;
  uint8_t repeat = 1;

  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "code") {
      code_hex = server.arg(i).c_str();
      code = strtoul(server.arg(i).c_str(), NULL, 16);
    }
    if (server.argName(i) == "repeat") {
      repeat = strtoul(server.arg(i).c_str(), NULL, 10);
    }
  }
  Serial.println("");
  Serial.print("Sending NEC '0x");
  Serial.print(code_hex.c_str());
  Serial.print("' [");
  Serial.print(code);
  Serial.print("], ");
  Serial.print(repeat);
  Serial.println(" times...");

#if SEND_NEC
  digitalWrite(LED_BUILTIN, LOW);
  irsend.sendNEC(code, 32, repeat);
  digitalWrite(LED_BUILTIN, HIGH);
#endif  // SEND_NEC
  server.send(200, "text/plain", "OK");  
  

}

void handleNotFound() {
  server.send(501, "text/plain", "API calls only");
}

void setup(void) {
  irsend.begin();

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/send", handleIr);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop(void) {
  server.handleClient();
}

