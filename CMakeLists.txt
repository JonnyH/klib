cmake_minimum_required(VERSION 2.8)
project(klib)
enable_testing()

option (OLTO "Enable Link Time Optimisations (LTO)" OFF)
option (ONATIVE "Optimise for the host CPU" OFF)
option (OPROFILE "Enable profiling (gcov, gprov) options for build" OFF)

if (OLTO)
		set(OPT_FLAGS "${OPT_FLAGS} -flto")
endif()
if (ONATIVE)
		set(OPT_FLAGS "${OPT_FLAGS} -march=native")
endif()
if (OPROFILE)
		set(OPT_FLAGS "${OPT_FLAGS} -pg -fprofile-arcs -ftest-coverage")
endif()

set(CMAKE_C_FLAGS "-std=gnu11 -fvisibility=hidden -pthread ${CMAKE_C_FLAGS} ${OPT_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=gnu++11 -fvisibility=hidden -pthread ${CMAKE_CXX_FLAGS} ${OPT_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-fvisibility=hidden -pthread ${CMAKE_MODULE_LINKER_FLAGS} ${OPT_FLAGS}")

set(KLIB_SOURCE_FILES
		src/krwlock.c
				)

set(KLIB_HEADER_FILES
		include/klib/khash.h
		include/klib/kstring.h
		include/klib/klist.h
		include/klib/krwlock.h
		include/klib/katomic.h
		)

include_directories(${klib_SOURCE_DIR}/include)

add_library(klib STATIC ${KLIB_SOURCE_FILES} ${KLIB_HEADER_FILES})

install (TARGETS klib DESTINATION lib)
install (FILES ${KLIB_HEADER_FILES} DESTINATION include/klib)

add_subdirectory(tests)
