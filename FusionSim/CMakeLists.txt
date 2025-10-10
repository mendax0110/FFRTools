cmake_minimum_required(VERSION 3.17)
project(FusionSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/../PotentialMap PotentialMap_build)
add_subdirectory(${CMAKE_SOURCE_DIR}/../SFPS SFPS_build)

add_subdirectory(src)

find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
    message(STATUS "Python3 gefunden: ${Python3_EXECUTABLE}")
endif()
