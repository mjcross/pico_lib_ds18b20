add_library(pico_lib_ds18b20 INTERFACE)

target_sources(pico_lib_ds18b20 INTERFACE
    ds18b20.c
    )

target_include_directories(pico_lib_ds18b20 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    )

target_link_libraries(pico_lib_ds18b20 INTERFACE 
    pico_stdlib
    pico_lib_onewire
    )