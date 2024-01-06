cmake_minimum_required(VERSION 2.4)
project(PotentialMap)

set(SOURCE_FILES
    ./src/main.cpp
    ./src/Colourisers.h
    ./src/Colourisers.cpp
    ./src/Calculators.h
    ./src/Calculators.cpp)

set(BOOST_ROOT "C:/boost_1_82_0")
find_package(Boost 1.82.0 REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Boost not found. Please set BOOST_ROOT to the correct Boost installation path.")
endif()

include_directories(C:\\boost_1_82_0\\boost)
link_directories(C:\\boost_1_82_0\\bin.v2\\libs)

# Change the output directory to build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Create the build version
add_executable(Build_${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(Build_${PROJECT_NAME} ${Boost_LIBRARIES})
