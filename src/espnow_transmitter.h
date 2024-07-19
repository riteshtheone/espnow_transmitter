#ifndef TRANSMITTER_WEB_ESPNOW_TRANSMITTER_H
#define TRANSMITTER_WEB_ESPNOW_TRANSMITTER_H

#include "Arduino.h"
#include "WiFi.h"
#include "esp_now.h"

const uint8_t receiverMac[] = {0x40, 0x91, 0x51, 0xFC, 0x52, 0xC8};

struct Signal {
    uint16_t throttle, yaw, pitch, roll, ax5;
};

Signal send_data;
esp_now_peer_info_t peerInfo;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    (void) mac_addr;
    if (status == ESP_NOW_SEND_SUCCESS) neopixelWrite(RGB_BUILTIN, 0, 0, 0);
    else neopixelWrite(RGB_BUILTIN, 5, 0, 0);
}

void transmitter_setup() {
    WiFiClass::mode(WIFI_MODE_STA);

    if (esp_now_init() != ESP_OK) {
        neopixelWrite(RGB_BUILTIN, 5, 0, 5);
        return;
    }
    esp_now_register_send_cb(onDataSent);
    memcpy(peerInfo.peer_addr, receiverMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        neopixelWrite(RGB_BUILTIN, 5, 0, 5);
        return;
    }

}
#endif //TRANSMITTER_WEB_ESPNOW_TRANSMITTER_H
