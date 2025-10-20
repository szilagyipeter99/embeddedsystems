#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAILED_BIT BIT1
#define MQTT_CONNECTED_BIT BIT2
#define MQTT_FAILED_BIT BIT3
#define MQTT_SUBSCRIBED_BIT BIT4

#define WIFI_CONNECTION_BITMASK (WIFI_CONNECTED_BIT | WIFI_FAILED_BIT)
#define MQTT_CONNECTION_BITMASK (MQTT_CONNECTED_BIT | MQTT_FAILED_BIT)

extern EventGroupHandle_t my_event_group;
extern QueueHandle_t led_queue;