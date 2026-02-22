#pragma once

#include "onewire.h"
#include "ds18b20_commands.h"

typedef union {
    struct {
        uint16_t temp_16ths:16;    // LSB of raw value
        uint8_t Th:8;
        uint8_t Tl:8;
        uint8_t config:8;
        uint8_t reserved[3];
    };
    uint8_t byte[8];
    uint64_t raw;
} ds18b20_scratchpad_t;

// public API
void ds18b20_start_convert_all(const onewire_t ow);
bool ds18b20_is_busy(const onewire_t ow);
float ds18b20_fetch_temperature(const onewire_t ow, const onewire_id_t *id_ptr);
bool ds18b20_fetch_temperature_check_crc(float *pTemp, const onewire_t ow, const onewire_id_t *id_ptr);
bool ds18b20_fetch_scratchpad_check_crc(ds18b20_scratchpad_t *pScratchpad, onewire_t ow, const onewire_id_t *id_ptr);