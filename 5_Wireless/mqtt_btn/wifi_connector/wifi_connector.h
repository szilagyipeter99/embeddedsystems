#pragma once

#include "esp_err.h"

void wifi_init_phase();
void wifi_config_phase();
esp_err_t wifi_start_phase(void);