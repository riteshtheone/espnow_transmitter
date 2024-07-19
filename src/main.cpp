#include "Arduino.h"
#include "ESPAsyncWebServer.h"
#include "page.h"
#include "espnow_transmitter.h"
#include "joystick_reading.h"

boolean debug = true;
AsyncWebServer server(80);

void serverSetup() {
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        return request->send(200, "text/html", page);
    });
    server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (!request->hasParam("q")) return request->send(404, "text/plain", "empty query");
        String event = request->getParam("q")->value();

        if (event.equals("ri") && middle_val_roll  < 1000) middle_val_roll++;
        if (event.equals("rd") && middle_val_roll  > 0)    middle_val_roll--;
        if (event.equals("pi") && middle_val_pitch < 1000) middle_val_pitch++;
        if (event.equals("pd") && middle_val_pitch > 0)    middle_val_pitch--;
        if (event.equals("yi") && middle_val_yaw   < 1000) middle_val_yaw++;
        if (event.equals("yd") && middle_val_yaw   > 0)    middle_val_yaw--;
        if (event.equals("ti") && middle_val_throttle < 1000) middle_val_throttle++;
        if (event.equals("td") && middle_val_throttle > 0)    middle_val_throttle--;

        if (event.equals("r1")) {
            middle_val_throttle = 406;
            middle_val_yaw  = 500;
        } 
        if (event.equals("r2")) middle_val_roll = middle_val_pitch = 500;

        return request->send(200, "text/plain", event);
    });
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
        const String s = "roll: "+ String(middle_val_roll) +" pitch: "+ String(middle_val_pitch) +" throttle: "+ String(middle_val_throttle) +" yaw: "+ String(middle_val_yaw);
        return request->send(200, "text/plain", s);
    });
    server.on("/throttle", HTTP_GET, [](AsyncWebServerRequest *request) {
        return request->send(200, "text/plain", String(middle_val_throttle));
    });
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Not found");
    });
    server.begin();
}

void wifiSetup() {
    WiFi.softAP("transmitter", "0123456789");
}

long transmission_timer, logTimer;
const int transmission_delay = 20;
const int logDelay = 200;

void setup() {
    if(debug) Serial.begin(115200);
    transmitter_setup();
    wifiSetup();
    serverSetup();

    pinMode(CH5PIN, INPUT_PULLUP);

    joystick_setup();
}

void loop() {
    if (millis() - transmission_timer >= transmission_delay) {
        transmission_timer = millis();
        read_joystick();
        send_data.throttle = throttle + 1000;
        send_data.yaw = yaw + 1000;
        send_data.pitch = pitch + 1000;
        send_data.roll = roll + 1000;
        send_data.ax5 = digitalRead(CH5PIN) ? 1000 : 1500;
        esp_now_send(receiverMac, (uint8_t *) &send_data, sizeof(send_data));
    }

   if (debug && millis() - logTimer >= logDelay) {
       logTimer = millis();
       Serial.print(roll + 1000);
       Serial.print(", ");
       Serial.print(pitch + 1000);
       Serial.print(", ");
       Serial.print(throttle + 1000);
       Serial.print(", ");
       Serial.print(yaw + 1000);
       Serial.print(", ");
       Serial.print(send_data.ax5);
       Serial.println();
   }
}