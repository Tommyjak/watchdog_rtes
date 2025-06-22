# Modify this line with the position of FreeRTOS-Kernel
set(FREERTOS_PATH "/home/tommaso/Documents/FreeRTOS-Kernel")

file(GLOB FREERTOS_KERNEL_SRC
    ${FREERTOS_PATH}/*.c
    ${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/port.c
    ${FREERTOS_PATH}/portable/MemMang/heap_4.c
)

add_executable(watchdog_rtes
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${FREERTOS_KERNEL_SRC}
)

target_include_directories(watchdog_rtes PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${FREERTOS_PATH}/include
    ${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/include
)

# FreeRTOS RP2040 port needs IRQ & exception headers from pico-sdk
target_include_directories(watchdog_rtes PRIVATE
    ${PICO_SDK_PATH}/src/common/pico_base/include
    ${PICO_SDK_PATH}/src/rp2_common/hardware_base/include
    ${PICO_SDK_PATH}/src/rp2_common/hardware_irq/include
    ${PICO_SDK_PATH}/src/rp2_common/hardware_exception/include
)

# pull in common dependencies
target_link_libraries(watchdog_rtes 
    pico_stdlib
    hardware_irq
    hardware_exception
)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(watchdog_rtes)

# add url via pico_set_program_url
example_auto_set_url(watchdog_rtes)

# enable USB output
pico_enable_stdio_usb(watchdog_rtes 1)
