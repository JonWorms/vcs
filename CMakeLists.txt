cmake_minimum_required (VERSION 3.7.2)
project (vcs)


option(USE_STANDALONE_ASIO "set to OFF to use boost instead of standalone Asio" ON)

#find_library(LIB_ALSA asound)
#if(NOT LIB_ALSA)
#	message(FATAL_ERROR "Could not find ALSA")
#else()
#	set(EXT_LIBS "asound")
#endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)



find_library(LIBSSL ssl)
if(NOT LIBSSL)
	message(FATAL_ERROR "could not find libssl")
endif()

find_library(LIBCRYPTO crypto)
if(NOT LIBCRYPTO)
	message(FATAL_ERROR "could not find libcrypto")
endif()



set (LIB lib)
add_subdirectory ("${LIB}/audio")
add_subdirectory ("${LIB}/stt")
add_subdirectory ("${LIB}/Simple-Web-Server")


# configure project ssl cert location
if(NOT SSL_CERT_DIRECTORY)
	message(STATUS "Setting ssl cert directory to binary path")
	set(SSL_CERT_DIRECTORY ${PROJECT_BINARY_DIR})
endif()
configure_file("ssl_cert_location.h.in" "${PROJECT_BINARY_DIR}/ssl_cert_location.h")




include_directories("${PROJECT_SOURCE_DIR}/include" ${PROJECT_BINARY_DIR} ${audio_INCLUDE_DIRS} ${stt_INCLUDE_DIRS})
add_executable( vcs "src/main.cpp" "src/interface.cpp" "src/client.cpp" "src/ring_buffer.cpp")

target_link_libraries(vcs audio stt simple-web-server)
target_link_libraries(vcs Threads::Threads)

