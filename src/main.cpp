#include <Arduino.h>
#include <WiFi.h>
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include <WebServer.h>
#include <DNSServer.h>

#define FLASH_GPIO_NUM GPIO_NUM_4
const char* ssid = "ESP_P_METER";
const char* password = "1234567890";

#define ESP_CONNECT_TIMEOUT 15000
#define SERVER_TIMEOUT 5000UL 

IPAddress apIP(192, 168, 1, 1);

const byte DNS_PORT = 53;
DNSServer dnsServer;

WebServer server(80);


esp_sleep_wakeup_cause_t print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
  return wakeup_reason;
}

void handleRoot();
void handleDHCP();
void handleStatic();
void handleNotFound();

void setup() {
    //WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    Serial.printf("Total PSRAM: %d\r\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\r\n", ESP.getFreePsram());

    print_wakeup_reason();  

    Serial.printf("Heap 1: %d\r\n", esp_get_free_heap_size());


    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

    if(!WiFi.softAP(ssid, password)){
      Serial.println(F("Failed to set up softAP mode!"));
    }

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // while (WiFi.status() != WL_CONNECTED) {
    //     Serial.print(".");
    //     delay(500);
    // }

    // Serial.println(F("WiFi connected"));
    // Serial.print(F("IP address: "));
    // Serial.println(WiFi.localIP());

    dnsServer.start(DNS_PORT, "*", apIP);
    Serial.printf("Heap 3: %d\r\n", esp_get_free_heap_size());


    // esp_sleep_enable_timer_wakeup(60 * 1000 * 1000); // 30s
    // ESP_LOGE("ESP", "Going to a deep sleep!");
    // esp_deep_sleep_start();
    server.begin();
    server.on("/", handleRoot); 
    server.on("/dhcp", handleDHCP);
    server.on("/static", handleStatic);
    server.onNotFound(handleNotFound);
}

void handleRoot() {

}

void handleDHCP(){

}

void handleStatic(){

}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  server.send(404, "text/plain", message);
}

void loop() {
    //delay(1000);
    // Serial.println("Client disconnected.");
    // Serial.println("");
}

