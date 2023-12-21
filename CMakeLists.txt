cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(espress_stream_project C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_compile_options(-Wall)
set(INCLUDES
	espress_stream/include
	espress_stream/api/include
	cipher/include
)
include_directories(${INCLUDES})
file(GLOB TEST_SOURCES
	test_main.c
	espress_stream/src/*.c
	espress_stream/api/src/*.c
)
file(GLOB APP_SOURCES
	app_main.c
	espress_stream/src/*.c
	espress_stream/api/src/*.c
	cipher/src/*.c
)

# espress_stream_test_lwip_poll

add_executable(espress_stream_test_lwip_poll ${TEST_SOURCES})

target_include_directories(espress_stream_test_lwip_poll PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(espress_stream_test_lwip_poll pico_stdlib pico_dividerpico_cyw43_arch_lwip_poll)

pico_enable_stdio_usb(espress_stream_test_lwip_poll 1)
pico_enable_stdio_uart(espress_stream_test_lwip_poll 0)

pico_add_extra_outputs(espress_stream_test_lwip_poll)

# espress_stream_test_lwip_background

add_executable(espress_stream_test_lwip_background ${TEST_SOURCES})

target_include_directories(espress_stream_test_lwip_background PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(espress_stream_test_lwip_background pico_stdlib pico_cyw43_arch_lwip_threadsafe_background)

pico_enable_stdio_usb(espress_stream_test_lwip_background 1)
pico_enable_stdio_uart(espress_stream_test_lwip_background 0)

pico_add_extra_outputs(espress_stream_test_lwip_background)

# espress_stream_app_lwip_poll

add_executable(espress_stream_app_lwip_poll ${APP_SOURCES})

target_compile_definitions(espress_stream_app_lwip_poll PRIVATE
	ESPRESS_STREAM_MAJOR_VERSION=0
	ESPRESS_STREAM_MINOR_VERSION=9
	ESPRESS_STREAM_PROTOCOL_VERSION=0)
target_include_directories(espress_stream_app_lwip_poll PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(espress_stream_app_lwip_poll pico_stdlib pico_cyw43_arch_lwip_poll)

pico_enable_stdio_usb(espress_stream_app_lwip_poll 1)
pico_enable_stdio_uart(espress_stream_app_lwip_poll 0)

pico_add_extra_outputs(espress_stream_app_lwip_poll)

# espress_stream_app_lwip_background

add_executable(espress_stream_app_lwip_background ${APP_SOURCES})

target_compile_definitions(espress_stream_app_lwip_background PRIVATE
	ESPRESS_STREAM_MAJOR_VERSION=0
	ESPRESS_STREAM_MINOR_VERSION=9
	ESPRESS_STREAM_PROTOCOL_VERSION=0)
target_include_directories(espress_stream_app_lwip_background PRIVATE
	${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(espress_stream_app_lwip_background pico_stdlib pico_cyw43_arch_lwip_threadsafe_background)

pico_enable_stdio_usb(espress_stream_app_lwip_background 1)
pico_enable_stdio_uart(espress_stream_app_lwip_background 0)

pico_add_extra_outputs(espress_stream_app_lwip_background)
