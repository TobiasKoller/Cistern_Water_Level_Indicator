
#ifndef ESP8266_TYPES_H
#define ESP8266_TYPES_H

#include <espnow.h>
#include <ESP8266WiFi.h>

// Definiere ESP32-Typen für ESP8266
typedef uint8_t esp_err_t;
typedef uint8_t esp_now_peer_info_t[6];
typedef uint8_t esp_now_send_status_t;

#define ESP_OK 0
#define ESP_NOW_SEND_SUCCESS 0

// Wrapper-Funktionen
inline esp_err_t esp_now_init_wrapper() {
    return esp_now_init();
}

inline void esp_now_register_send_cb_wrapper(void (*cb)(uint8_t*, uint8_t)) {
    esp_now_register_send_cb(cb);
}

inline esp_err_t esp_now_add_peer_wrapper(const uint8_t* peer_addr, uint8_t role, uint8_t channel, const uint8_t* key, uint8_t key_len) {
    return esp_now_add_peer(peer_addr, role, channel, key, key_len);
}

inline esp_err_t esp_now_send_wrapper(const uint8_t* peer_addr, const uint8_t* data, size_t len) {
    return esp_now_send(peer_addr, data, len);
}

#endif 