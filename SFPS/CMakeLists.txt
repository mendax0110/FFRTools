cmake_minimum_required(VERSION 2.4)
project(SFPS)

set(SOURCE_FILES
    src/main.cpp
    src/Configuration.cpp
    src/Proton.cpp
    src/Quantities.cpp
)

set(HEADER_FILES
    src/Calculations.h
    src/CollisionModel.h
    src/Configuration.h
    src/FieldVector.h
    src/MappedFieldModel.h
    src/MicroMetrePoint.h
    src/PointMap.h
    src/Proton.h
    src/Quantities.h
    src/Scalar.h
    src/Vector3d.h
    src/while_moving.h
)

if(WIN32)
    set(CMAKE_CXX_STANDARD 11)
    set(BOOST_ROOT "C:/boost_1_82_0")
else()
    set(CMAKE_CXX_STANDARD 11)
    set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.83.0")
endif()

if(WIN32)
    find_package(Boost 1.82.0 REQUIRED COMPONENTS program_options)
else()
    find_package(Boost 1.83.0 REQUIRED COMPONENTS program_options)
endif()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "Boost not found. Please set BOOST_ROOT to the correct Boost installation path.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
