cmake_minimum_required (VERSION 3.7.2)

project (vcs)

set (PROJECT_SOURCE_DIR src)
set (PROJECT_INCLUDE_DIR include)

set (EXE_BUILD_STRING "${PROJECT_SOURCE_DIR}/main.c")


find_library(LIB_ALSA asound)
if(NOT LIB_ALSA)
	message(FATAL_ERROR "Could not find ALSA")
else()
	set(EXT_LIBS "asound")
endif()



add_executable (vcs "${EXE_BUILD_STRING}")
target_link_libraries(vcs "${EXT_LIBS}")