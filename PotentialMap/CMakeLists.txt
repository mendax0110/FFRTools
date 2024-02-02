cmake_minimum_required(VERSION 3.0)
project(PotentialMap)

set(SOURCE_FILES
    ./src/main.cpp
    ./src/Colourisers.h
    ./src/Colourisers.cpp
    ./src/Calculators.h
    ./src/Calculators.cpp
    ./src/CalcMacros.h)

if(WIN32)
    set(CMAKE_CXX_STANDARD 11)
    set(BOOST_ROOT "C:/boost_1_82_0")
    find_package(Boost 1.82.0 REQUIRED)
else()
    set(CMAKE_CXX_STANDARD 11)
    set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.83.0")
    find_package(Boost 1.83.0 REQUIRED)
    find_package(JPEG REQUIRED)
    find_package(PNG REQUIRED)
endif()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
    include_directories(${JPEG_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${JPEG_LIBRARIES})
    include_directories(${PNG_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARIES})
else()
    message(FATAL_ERROR "Boost not found. Please set BOOST_ROOT to the correct Boost installation path.")
endif()

if(WIN32)
    include_directories(C:\\boost_1_82_0\\boost)
    link_directories(C:\\boost_1_82_0\\bin.v2\\libs)
else()
    include_directories(/opt/homebrew/Cellar/boost/1.83.0/include)
    link_directories(/opt/homebrew/Cellar/boost/1.83.0/lib)
    link_libraries(/opt/homebrew/opt/jpeg/lib/libjpeg.dylib)
    link_libraries(/opt/homebrew/opt/libpng/lib/libpng.dylib)
endif()

# Change the output directory to build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Create the build version
add_executable(Build_${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(Build_${PROJECT_NAME} ${Boost_LIBRARIES})
