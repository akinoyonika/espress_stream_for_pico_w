cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(espress_stream_project C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

set(SUBDIRECTORIES
	espress_stream
)

set(OPTIONS
	-Wall
)

set(INCLUDES
	espress_stream/api/include
	cipher/include
)

file(GLOB TEST_SOURCES
	test_main.c
)

file(GLOB ES_FTX_SOURCES
	es_ftx_main.c
	cipher/src/*.c
)

file(GLOB ES_CCAT_SOURCES
	es_ccat_main.c
	cipher/src/*.c
)

set(LIBRARIES
	pico_stdlib
	pico_cyw43_arch_lwip_threadsafe_background
	espress_stream
)

foreach(dir ${SUBDIRECTORIES})
	add_subdirectory(${dir})
endforeach()

# test

add_executable(test ${TEST_SOURCES})
target_compile_options(test PRIVATE ${OPTIONS})
target_include_directories(test PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${INCLUDES})
target_link_libraries(test ${LIBRARIES})

pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 0)

pico_add_extra_outputs(test)

# es_ftx

add_executable(es_ftx ${ES_FTX_SOURCES})
target_compile_options(es_ftx PRIVATE ${OPTIONS})
target_include_directories(es_ftx PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${INCLUDES})
target_link_libraries(es_ftx ${LIBRARIES})

pico_enable_stdio_usb(es_ftx 1)
pico_enable_stdio_uart(es_ftx 0)

pico_add_extra_outputs(es_ftx)

# es_ccat

add_executable(es_ccat ${ES_CCAT_SOURCES})
target_compile_options(es_ccat PRIVATE ${OPTIONS})
target_include_directories(es_ccat PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${INCLUDES})
target_link_libraries(es_ccat ${LIBRARIES})

pico_enable_stdio_usb(es_ccat 1)
pico_enable_stdio_uart(es_ccat 0)

pico_add_extra_outputs(es_ccat)
