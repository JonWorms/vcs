cmake_minimum_required(VERSION 3.7.2)
find_library(LIB_ALSA asound)
if(NOT LIB_ALSA)
	message(FATAL_ERROR "Could not find ALSA")
endif()
include_directories("include")

add_library(audio src/device.cpp)
target_link_libraries(audio "asound")