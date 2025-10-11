cmake_minimum_required(VERSION 3.5)
project(PotentialMap)

set(SOURCE_FILES
    ./src/Colourisers.cpp
    ./src/Calculators.cpp
    ./src/GeneralEE.cpp
)
set(HEADER_FILES
    ./src/Colourisers.h
    ./src/Calculators.h
    ./src/CalcMacros.h
    ./src/GeneralEE.h
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(PotentialMapLib STATIC ${SOURCE_FILES} ${HEADER_FILES})

if(TARGET Boost::program_options)
    target_link_libraries(PotentialMapLib PUBLIC Boost::program_options)
else()
    target_link_libraries(PotentialMapLib PUBLIC ThirdParty)
endif()

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)

if(JPEG_FOUND)
    target_link_libraries(PotentialMapLib PUBLIC ${JPEG_LIBRARIES})
    target_include_directories(PotentialMapLib PUBLIC ${JPEG_INCLUDE_DIRS})
endif()

if(PNG_FOUND)
    target_link_libraries(PotentialMapLib PUBLIC ${PNG_LIBRARIES})
    target_include_directories(PotentialMapLib PUBLIC ${PNG_INCLUDE_DIRS})
endif()

target_include_directories(PotentialMapLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
