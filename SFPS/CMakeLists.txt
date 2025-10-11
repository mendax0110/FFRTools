cmake_minimum_required(VERSION 3.5)
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

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SFPSLib STATIC ${SOURCE_FILES} ${HEADER_FILES})

if(TARGET Boost::program_options)
    target_link_libraries(SFPSLib PUBLIC Boost::program_options)
else()
    target_link_libraries(SFPSLib PUBLIC ThirdParty)
endif()

target_include_directories(SFPSLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
