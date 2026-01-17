#pragma once

#include "onewire.h"
#include "ds18b20_commands.h"

// public API
void ds18b20_start_convert_all(onewire_t instance);
bool ds18b20_is_busy(onewire_t instance);
float ds18b20_fetch_temperature(onewire_t instance, onewire_id_t id);