#pragma once

#include "esp_err.h"

typedef struct
{
    uint8_t player_id;
    uint8_t payload;
} ir_packet_t;

esp_err_t ir_init(void);
esp_err_t ir_send(const ir_packet_t *packet);
bool ir_receive(ir_packet_t *packet, uint32_t timeout_ms);
