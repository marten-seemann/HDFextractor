cmake_minimum_required(VERSION 2.8)
project(HDFextractor C CXX)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeScripts)
include(CheckIncludeFile)

set(BUILD_SHARED_LIBRARIES OFF)
set(CMAKE_BUILD_TYPE "RELEASE")

file(GLOB CPPS src/*.cpp src/*.c src/*.hpp extern/*.cpp extern/*/*.cpp)
file(GLOB HEADERS include/*.h)

include_directories(include .)
include_directories(extern .)


find_package(Boost REQUIRED COMPONENTS system program_options filesystem)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Gnuplot)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIR})
message(STATUS ${ZLIB_INCLUDE_DIR})

find_library(HDF5_LIBRARY1 NAMES hdf5 PATH_SUFFIXES hdf5)
find_library(HDF5_LIBRARY2 NAMES hdf5_cpp PATH_SUFFIXES hdf5)
find_file(H5cpp H5Cpp.h PATH_SUFFIXES cpp)
get_filename_component(HDF5_INCLUDE_DIR ${H5cpp} PATH)
include_directories(${HDF5_INCLUDE_DIR})
message(STATUS ${HDF5_INCLUDE_DIR})
message(STATUS ${ZLIB_LIBRARY})

add_executable(HDFextractor ${HEADERS} ${CPPS})
target_link_libraries(HDFextractor ${Boost_LIBRARIES})
target_link_libraries(HDFextractor ${ZLIB_LIBRARY})
target_link_libraries(HDFextractor ${HDF5_LIBRARY1})
target_link_libraries(HDFextractor ${HDF5_LIBRARY2})

install (TARGETS HDFextractor DESTINATION bin CONFIGURATIONS Release)