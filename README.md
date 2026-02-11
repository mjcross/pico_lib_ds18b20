# DS18B20 temperature sensor library for the RPi Pico and Pico 2 'C' SDK

A library that sits alongside my [pico_lib_onewire](https://github.com/mjcross/pico_lib_onewire) to provide convenience functions for working with the DS18B20 digital temperature sensor.

For a demonstration of how to use the libraries see [pico_ds18b20_example](https://github.com/mjcross/pico_ds18b20_example).

## about the device
The [DS18B20](https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf) is an inexpensive digital temperature sensor with a resolution of about 0.1ºC and an absolute accuracy of +/- 0.5ºC. It is widely available as either a discrete transistor-style package (TO-92) or in a waterproof 6mm stainless steel encapsulation with a 3 core cable.

The cable colours for the wired sensors are typically:

| wire colour| function|
|---|---|
| Red|  Vcc (3.3v)|
| Yellow | 1-Wire data|
| Black | Gnd (0v)

With the 1-Wire interface you can control (almost) as many devices as you like from a single GPIO. Simply by wire all the data lines together to the pin and pull it up to the positive supply with a 4.7k resistor (note that the internal gpio 'pull-up' is not really suitable for this).

Each device has a unique 64bit id that enables the controller to addresss it individually.

Do not power the devices at more than 3.3V as it will damage the GPIO.

## importing the library
To use this library you will have to import both it and [pico_lib_onewire](https://github.com/mjcross/pico_lib_onewire) into your project.

### Repeat the following steps for each library:

Fork the repository on GitHub or clone a local copy. Then change to your project directory and do:
```
$ git submodule add <path-to-library-repository or URL>
```

Note that anyone cloning your repositiory should now use `git clone --recursive` to make sure they get the full source tree.

## including the library in your build
Once you've imported both libraries you can include them in your CMake build by putting the following lines in your top level CMakeLists.txt:
```
add_subdirectory(pico_lib_onewire)
add_subdirectory(pico_lib_ds18b20)
target_link_libraries(<project-name> 
    pico_lib_onewire
    pico_lib_ds18b20
    )
```
You should now be able to add
```
#include "onewire.h"
#include "ds18b20.h"
```
to your source code and start using the functions from either library.

## using the library
As noted above this library just adds convenience functions for working with the DS18B20: you'll still need to use the [pico_lib_onewire](https://github.com/mjcross/pico_lib_onewire) API for basic functions like initialising and scanning the bus.

The functions added are:

`void ds18b20_start_convert_all(const onewire_t ow)`

Assert a reset condition, then instruct all connected devices to start a temperature conversion.

**Inputs:** 
- an initialised instance of the onewire library

**Outputs:** 
- none

The temperature conversion process takes about a second for the default 12-bit resolution mode. To check whether all devices have finished their conversion use `ds18b20_is_busy()` (see below).

----
`bool ds18b20_is_busy(const onewire_t ow)`

Check whether all devices have finished a temperature conversion.

**Inputs:**
- onewire library instance

**Outputs:**
- returns `true` if any device is still doing a conversion, or `false` when all devices have finished.

The function uses the behaviour that a DS18B20 device doing a temperature conversion will pull the bus low in response to the master generating a 'read' timeslot (see the [datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ds18b20.pdf)).

----
`float ds18b20_fetch_temperature(const onewire_t ow, const onewire_id_t *id_ptr)`

Read the result of a temperature conversion on an individual device.

**Inputs:**
- onewire library instance
- pointer to a device ID

**Outputs:**
- returns the temperature in degrees centrigrade

The device actually returns the temperature in 1/16ths of a degree as a signed 16-bit integer, which the function converts to a float. You may prefer to use the raw value if you want to avoid using floating point arithmetic in your binary.
