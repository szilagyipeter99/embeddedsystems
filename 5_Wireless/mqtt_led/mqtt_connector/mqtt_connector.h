#pragma once

#include "esp_err.h"

esp_err_t mqtt_init();
esp_err_t mqtt_subscribe_to_led();

