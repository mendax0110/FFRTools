cmake_minimum_required(VERSION 3.10)

project(SFPS)

set(CMAKE_CXX_STANDARD 17)

set(MAIN_SOURCES
    src/main.cpp
    src/Configuration.cpp
    src/Proton.cpp
    src/Quantities.cpp
)

set(MAIN_HEADERS
    src/Calculations.h
    src/CollisionModel.h
    src/Configuration.h
    src/FieldVector.h
    src/MappedFieldModel.h
    src/MicroMeterPoint.h
    src/PointMap.h
    src/Proton.h
    src/Quantities.h
    src/Scalar.h
    src/Vector3d.h
    src/while_moving.h
)

# create executable
add_executable(SFPS ${MAIN_SOURCES} ${MAIN_HEADERS})

# add include directories
target_include_directories(SFPS PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)