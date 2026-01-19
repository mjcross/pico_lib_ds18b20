#pragma once

#include "onewire.h"
#include "ds18b20_commands.h"

// public API
void ds18b20_start_convert_all(const onewire_t ow);
bool ds18b20_is_busy(const onewire_t ow);
float ds18b20_fetch_temperature(const onewire_t ow, const onewire_id_t *id_ptr);