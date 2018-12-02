cmake_minimum_required (VERSION 3.7.2)

project (vcs)

set (PROJECT_SRC_DIR src)
set (PROJECT_INC_DIR include)
set (PROJECT_LIB_DIR lib)


#find_library(LIB_ALSA asound)
#if(NOT LIB_ALSA)
#	message(FATAL_ERROR "Could not find ALSA")
#else()
#	set(EXT_LIBS "asound")
#endif()


set (AUDIO "${PROJECT_LIB_DIR}/audio")

include_directories(${PROJECT_INC_DIR}
					"${AUDIO}/include")


add_subdirectory (${AUDIO})
set (LIBS ${LIBS} audio)




set (MAIN "${PROJECT_SRC_DIR}/main.cpp")

add_executable( vcs ${MAIN} )

target_link_libraries(vcs ${LIBS})