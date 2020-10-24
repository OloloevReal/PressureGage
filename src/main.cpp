#include <Arduino.h>
#include <WiFi.h>
#include "WebServer.h"
#include <DNSServer.h>
#include "web.h"
#include "sensor.h"
#include "setup.h"

Sensor sp(PIN_ADC);
DNSServer dnsServer;
WebServer server(80);

void notFound() {
    server.send(404, FPSTR(cType_PLAIN), F("Not found"));
}

void handleStates(){
  const sensor_value_t *value = sp.getValue();
  char buf[300];
  sprintf_P(buf, valuesJSON
            , value->pa
            , value->volt
            , value->bar
            , value->bar>=0?COLOR_HIGH:COLOR_LOW);
  server.send(200, FPSTR(cType_JSON), buf);
}

void handleGetParams(){
  char buf60[60];
  sensor_params_t params = sp.getParams();
  sprintf_P(buf60, paramsJSON, params._zero_level, params.volt_offset);
  server.send(200, FPSTR(cType_JSON), buf60);
}

void handleSetParams()
{
  String zl = String(T_zeroLevel);
  String vo = String(T_voltOffset);
  if (!server.hasArg(zl) || !server.hasArg(vo)){
    Serial.printf_P(PSTR("POST request, params '%s' or '%s' not found\r\n"), T_voltOffset, T_zeroLevel);
    server.send(422, FPSTR(cType_JSON), F("{\"error\":\"Unprocessable Entity\"}"));
    return;
  }
  String zl_value = server.arg(zl);
  sp.setZeroLevel(zl_value);
  String vo_value = server.arg(vo);
  sp.setVoltOffset(vo_value);
  if(zl_value.length() == 0 || vo_value.length() == 0){
      server.send(400, FPSTR(cType_JSON), F("{\"error\":\"Bad Request\"}"));
      return;
  }

  sensor_params_t param = sp.getParams();
  char buf60[60];
  sprintf_P(buf60, paramsJSON, param._zero_level, param.volt_offset);
  server.send(200, FPSTR(cType_JSON), buf60);
}

void handleRestore(){
    sp.restore();
    handleGetParams();
}

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    print_wakeup_reason();  
    //check_efuse();

    if (sp.begin() != ESP_OK){
        Serial.println(F("Failed to start up ADC measurement!"));
    }

    WiFi.softAP(AP_NAME);
    Serial.printf("WiFi name is %s\r\n", AP_NAME);
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
    
    server.on("/", HTTP_GET, [](){
        server.send_P(200, cType_HTML, HTML_INDEX_PAGE);
    });

    server.on("/states", HTTP_GET, handleStates);
    server.on("/params", HTTP_GET, handleGetParams);
    server.on("/params", HTTP_POST, handleSetParams);
    server.on("/restore", HTTP_GET, handleRestore);

    server.onNotFound(notFound);

    dnsServer.start(53, "*", WiFi.softAPIP());
    server.begin();

    Serial.println("Server started!");
}

void loop() {
  server.handleClient();
  dnsServer.processNextRequest();
}

