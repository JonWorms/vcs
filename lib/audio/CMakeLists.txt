cmake_minimum_required(VERSION 3.7.2)
project(audio)


find_library(LIB_ALSA asound)
if(NOT LIB_ALSA)
	message(FATAL_ERROR "Could not find ALSA")
endif()

find_path(LIB_ALSA_INCLUDE "alsa/asoundlib.h")
if(NOT LIB_ALSA_INCLUDE)
	message(FATAL_ERROR "could not locate alsa header files")
endif()
set(LIB_ALSA_INCLUDE "${LIB_ALSA_INCLUDE}/alsa")

find_library(LIB_SNDFILE sndfile)
if(NOT LIB_SNDFILE)
	message(FATAL_ERROR "Could not find libsndfile")
endif()

find_path(LIB_SNDFILE_INCLUDE "sndfile.h")
if(NOT LIB_SNDFILE_INCLUDE)
	message(FATAL_ERROR "could not locate libsndfile header files")
endif()

message(STATUS ${LIB_SNDFILE_INCLUDE})
message(STATUS ${LIB_ALSA_INCLUDE})

include_directories(${PROJECT_BINARY_DIR} "${PROJECT_SOURCE_DIR}/include" ${LIB_ALSA_INCLUDE} ${LIB_SNDFILE_INCLUDE})


set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/include ${LIB_ALSA_INCLUDE} ${LIB_SNDFILE_INCLUDE}
CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE) 

add_library(audio src/device.cpp src/file.cpp src/stream.cpp src/output_stream.cpp src/input_stream.cpp)
target_link_libraries(audio "asound" "sndfile")