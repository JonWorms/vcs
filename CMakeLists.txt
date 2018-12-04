cmake_minimum_required (VERSION 3.7.2)

project (vcs)

set (LIB lib)


#find_library(LIB_ALSA asound)
#if(NOT LIB_ALSA)
#	message(FATAL_ERROR "Could not find ALSA")
#else()
#	set(EXT_LIBS "asound")
#endif()


add_subdirectory ("${LIB}/audio")
add_subdirectory ("${LIB}/stt")

include_directories(${PROJECT_BINARY_DIR} ${audio_INCLUDE_DIRS} ${stt_INCLUDE_DIRS})
add_executable( vcs "src/main.cpp" )
target_link_libraries(vcs audio stt)

