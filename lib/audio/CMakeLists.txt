cmake_minimum_required(VERSION 3.7.2)
find_library(LIB_ALSA asound)
if(NOT LIB_ALSA)
	message(FATAL_ERROR "Could not find ALSA")
endif()


find_library(LIB_SOUNDFILE sndfile)
if(NOT LIB_SOUNDFILE)
	message(FATAL_ERROR "Could not find libsndfile")
endif()

include_directories("include")


add_library(audio src/device.cpp src/file.cpp src/stream.cpp src/output_stream.cpp src/input_stream.cpp)
target_link_libraries(audio "asound" "sndfile")