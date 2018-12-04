cmake_minimum_required(VERSION 3.7.2)
project(stt)


# FIND CMUSPHINX LIBRARIES AND INCLUDE PATHS
# pocketsphinx lib
find_library(POCKETSPHINX NAMES pocketsphinx)
if(NOT POCKETSPHINX) 
	message(FATAL_ERROR "could not find pocketsphinx")
endif()

# pocketsphinx include
find_path(POCKETSPHINX_INCLUDE "pocketsphinx/pocketsphinx.h")
if(NOT POCKETSPHINX_INCLUDE)
	message(FATAL_ERROR "could not find pocketsphinx header files")
endif()
set(POCKETSPHINX_INCLUDE "${POCKETSPHINX_INCLUDE}/pocketsphinx")


# sphinxbase lib
find_library(SPHINXBASE sphinxbase)
if(NOT SPHINXBASE)
	message(FATAL_ERROR "could not find sphinxbase")
endif()

# sphinxbase include
find_path(SPHINXBASE_INCLUDE "sphinxbase/sphinx_config.h")
if(NOT SPHINXBASE_INCLUDE)
	message(FATAL_ERROR "could not find sphinxbase header files")
endif()
set(SPHINXBASE_INCLUDE "${SPHINXBASE_INCLUDE}/sphinxbase")


# sphinxad lib
find_library(SPHINXAD sphinxad)
if(NOT SPHINXAD)
	message(FATAL_ERROR "could not find sphinxad")
endif()



# CONFIGURE CMUSPHINX MODEL PATH 
set(CMUSPHINX_MODEL_DIR "/usr/share/pocketsphinx/model")
configure_file ("stt_config.hpp.in" "${PROJECT_BINARY_DIR}/stt_config.hpp")



include_directories(${PROJECT_BINARY_DIR} "${PROJECT_SOURCE_DIR}/include" ${POCKETSPHINX_INCLUDE} ${SPHINXBASE_INCLUDE})

# export include paths to cache for projects that use libstt
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/include ${POCKETSPHINX_INCLUDE} ${SPHINXBASE_INCLUDE}
CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE) 


add_library(stt src/hotword_parser.cpp src/language_model.cpp src/engine.cpp)
target_link_libraries(stt ${POCKETSPHINX} ${SPHINXBASE} ${SPHINXAD})
target_link_libraries(stt "stdc++fs")

