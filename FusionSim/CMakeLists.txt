cmake_minimum_required(VERSION 3.17)
project(FusionSim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.88 REQUIRED COMPONENTS program_options)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost 1.88 not found. Install or point BOOST_ROOT appropriately.")
endif()

add_library(ThirdParty INTERFACE)
target_include_directories(ThirdParty INTERFACE ${Boost_INCLUDE_DIRS})
target_compile_definitions(ThirdParty INTERFACE ${Boost_DEFINITIONS})

add_subdirectory(${CMAKE_SOURCE_DIR}/../PotentialMap ${CMAKE_BINARY_DIR}/PotentialMap_build)
add_subdirectory(${CMAKE_SOURCE_DIR}/../SFPS ${CMAKE_BINARY_DIR}/SFPS_build)

add_subdirectory(src)

find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
    message(STATUS "Python3 gefunden: ${Python3_EXECUTABLE}")
endif()
