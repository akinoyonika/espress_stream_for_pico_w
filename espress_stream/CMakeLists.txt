set(OPTIONS
	-Wall
)

set(DEFINITIONS
	ESPRESS_STREAM_MAJOR_VERSION=1
	ESPRESS_STREAM_MINOR_VERSION=12
	ESPRESS_STREAM_PROTOCOL_VERSION=1
)

set(INCLUDES
	include
	api/include
)

file(GLOB SOURCES
	src/*.c
	api/src/*.c
)

set(LIBRARIES
	pico_stdlib
	pico_cyw43_arch_lwip_threadsafe_background
)

add_library(espress_stream STATIC ${SOURCES})
target_compile_options(espress_stream PRIVATE ${OPTIONS})
target_compile_definitions(espress_stream PRIVATE ${DEFINITIONS})
target_include_directories(espress_stream PRIVATE ${CMAKE_CURRENT_LIST_DIR} ${INCLUDES})
target_link_libraries(espress_stream ${LIBRARIES})
