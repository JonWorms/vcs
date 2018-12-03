cmake_minimum_required (VERSION 3.7.2)

project (vcs)

set (LIB lib)


#find_library(LIB_ALSA asound)
#if(NOT LIB_ALSA)
#	message(FATAL_ERROR "Could not find ALSA")
#else()
#	set(EXT_LIBS "asound")
#endif()


set (AUDIO "${LIB}/audio")
add_subdirectory (${AUDIO})

set (STT "${LIB}/stt")
add_subdirectory (${STT})

include_directories("${AUDIO}/include"
					"${STT}/include"
					"${PROJECT_BINARY_DIR}")



add_executable( vcs "src/main.cpp" )

target_link_libraries(vcs "audio" "stt")