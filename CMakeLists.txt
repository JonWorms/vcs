cmake_minimum_required (VERSION 3.7.2)

project (vcs)

set (LIB lib)


#find_library(LIB_ALSA asound)
#if(NOT LIB_ALSA)
#	message(FATAL_ERROR "Could not find ALSA")
#else()
#	set(EXT_LIBS "asound")
#endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)




add_subdirectory ("${LIB}/audio")
add_subdirectory ("${LIB}/stt")



include_directories("${PROJECT_SOURCE_DIR}/include" ${PROJECT_BINARY_DIR} ${audio_INCLUDE_DIRS} ${stt_INCLUDE_DIRS})
add_executable( vcs "src/main.cpp" "src/client.cpp" "src/ring_buffer.cpp")
target_link_libraries(vcs audio stt)
target_link_libraries(vcs Threads::Threads)

