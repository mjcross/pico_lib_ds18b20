#include "ds18b20.h"

// start temperature conversion in parallel on all devices
// (see ds18b20 datasheet)
void ds18b20_start_convert_all(const onewire_t ow) {
    onewire_reset(ow);                          // begin new command
    onewire_send(ow, OW_SKIP_ROM);              // address all devices
    onewire_send(ow, DS18B20_CONVERT_T);        // start temperature conversion
}

// check whether any devices are busy
bool inline ds18b20_is_busy(const onewire_t ow) {
    return (onewire_read(ow) == 0);         // the ds18b20 holds the bus low during a conversion
}

// read the temperature without checking the CRC
// slightly faster because we don't have to read the whole scratchpad
float ds18b20_fetch_temperature(onewire_t ow, const onewire_id_t *id_ptr) {
    onewire_reset(ow);                          // begin new commmand
    onewire_send(ow, OW_MATCH_ROM);             // address individual device
    for (int i = 0; i < 8; i += 1) {            // send device id
        onewire_send(ow, id_ptr->byte[i]);
    }
    onewire_send (ow, DS18B20_READ_SCRATCHPAD);

    int16_t t = 0;
    t = onewire_read(ow) | (onewire_read(ow) << 8); // read first two bytes of memory only
    return t / 16.0;
}

// read temperature and check the CRC
// returns: true if the crc matched
bool ds18b20_fetch_temperature_check_crc(float *temp_ptr, const onewire_t ow, const onewire_id_t *id_ptr) {
    bool crc_matched = false;
    *temp_ptr = 0;
    ds18b20_scratchpad_t scratchpad;
    if (ds18b20_fetch_scratchpad_check_crc(&scratchpad, ow, id_ptr)) {
        *temp_ptr = scratchpad.temp_16ths / 16.0;
        crc_matched = true;
    }
    return crc_matched;
}


// read the scratchpad (8 bytes + crc)
// returns: true if the CRC matched
bool ds18b20_fetch_scratchpad_check_crc(ds18b20_scratchpad_t *pScratchpad, onewire_t ow, const onewire_id_t *id_ptr) {
    onewire_reset(ow);                          // begin new commmand
    onewire_send(ow, OW_MATCH_ROM);             // address individual device
    for (int i = 0; i < 8; i += 1) {            // send device id
        onewire_send(ow, id_ptr->byte[i]);
    }
    onewire_send (ow, DS18B20_READ_SCRATCHPAD); // read all 8 bytes of the scratchpad
    for (int i = 0; i < 8; i += 1) {
        pScratchpad->byte[i] = onewire_read(ow);
    }
    uint8_t read_crc = onewire_read(ow);        // read the CRC (9th byte)
    
    // calculate crc from 64bit scratchpad (NOT a 56bit onewire romcode)
    uint8_t crc = 0;
    for(int i = 0; i < 64; i+= 1) {
        if (((pScratchpad->raw >> i) & 0x01) ^ (crc & 0x01)) {
            crc = (crc >> 1) ^ 0x8c;                                // CRC polynomial x^8 + x^5 + x^4 + 1
        } else {
            crc >>= 1;
        }
    }

    return (crc == read_crc);
}