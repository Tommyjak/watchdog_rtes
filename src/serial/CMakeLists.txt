add_executable(watchdog_rtes
	main.c
)

# pull in common dependencies
target_link_libraries(watchdog_rtes pico_stdlib)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(watchdog_rtes)

# add url via pico_set_program_url
example_auto_set_url(watchdog_rtes)

pico_enable_stdio_usb(watchdog_rtes 1)
