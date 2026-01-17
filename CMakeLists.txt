add_library(lib_ds18b20 INTERFACE)

target_sources(lib_ds18b20 INTERFACE
    ds18b20.c
    )

target_include_directories(lib_ds18b20 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    )

target_link_libraries(lib_ds18b20 INTERFACE 
    pico_stdlib
    lib_onewire
    )