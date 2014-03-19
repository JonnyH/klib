cmake_minimum_required(VERSION 2.8)
project(klib)

set(CMAKE_C_FLAGS "-std=gnu99 -fvisibility=hidden -pthread ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=gnu++0x -fvisibility=hidden -pthread
${CMAKE_CXX_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-fvisibility=hidden -pthread
${CMAKE_MODULE_LINKER_FLAGS}")

set(KLIB_SOURCE_FILES
		src/khashtable.c
		src/kstring.c
		src/klist.c
				)

set(KLIB_HEADER_FILES
		include/klib/khashtable.h
		include/klib/kstring.h
		include/klib/klist.h
		)

include_directories(${klib_SOURCE_DIR}/include)

add_library(klib SHARED ${KLIB_SOURCE_FILES})

install (TARGETS klib DESTINATION lib)
install (FILES ${KLIB_HEADER_FILES} DESTINATION include/klib)
