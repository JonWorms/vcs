cmake_minimum_required(VERSION 3.7.2)




# pocketsphinx
find_library(POCKET_SPHINX pocketsphinx)
if(NOT POCKET_SPHINX) 
	message(FATAL_ERROR "could not find pocketsphinx")
endif()

find_library(SPHINX_BASE sphinxbase)
if(NOT SPHINX_BASE)
	message(FATAL_ERROR "could not find sphinxbase")
endif()

find_library(SPHINX_AD sphinxad)
if(NOT SPHINX_AD)
	message(FATAL_ERROR "could not find sphinxad")
endif()


#/usr/share/pocketsphinx/model/

set(CMUSPHINX_MODEL_DIR "/usr/share/pocketsphinx/model")
configure_file ("stt.hpp.in" "${PROJECT_BINARY_DIR}/stt.hpp")

include_directories("${PROJECT_BINARY_DIR}" "include" "/usr/include/pocketsphinx" "/usr/include/arm-linux-gnueabihf/sphinxbase")

message(STATUS "${PROJECT_BINARY_DIR}/stt.hpp")


add_library(stt src/hotword_listener.cpp src/language_model.cpp)
target_link_libraries(stt "pocketsphinx" "sphinxbase" "sphinxad")