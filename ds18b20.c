#include "ds18b20.h"

// start temperature conversion in parallel on all devices
// (see ds18b20 datasheet)
void ds18b20_start_convert_all(const onewire_t ow) {
    onewire_reset(ow);                      // start command
    onewire_send(ow, OW_SKIP_ROM);          // address all devices
    onewire_send(ow, DS18B20_CONVERT_T);    // start temperature conversion
}

// check whether any devices are busy
bool inline ds18b20_is_busy(const onewire_t ow) {
    return (onewire_read(ow) == 0);         // the ds18b20 holds the bus low during a conversion
}

// read result of temperature conversion
float ds18b20_fetch_temperature(const onewire_t ow, const onewire_id_t *id_ptr) {
    onewire_reset(ow);                      // start commmand
    onewire_send(ow, OW_MATCH_ROM);         // address given device
    for (int i = 0; i < 8; i += 1) {        // send device id
        onewire_send(ow, id_ptr->byte[i]);
    }
    onewire_send (ow, DS18B20_READ_SCRATCHPAD); // read result of temperature conversion
    int16_t t = 0;
    t = onewire_read(ow) | (onewire_read(ow) << 8);
    return t / 16.0;
}