cmake_minimum_required(VERSION 3.5)
project(PotentialMap)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
    ./src/main.cpp
    ./src/Colourisers.h
    ./src/Colourisers.cpp
    ./src/Calculators.h
    ./src/Calculators.cpp
    ./src/CalcMacros.h
    ./src/GeneralEE.cpp
    ./src/GeneralEE.h
)

if(WIN32)

    set(CMAKE_CXX_STANDARD 11)
    set(BOOST_ROOT "C:/boost_1_82_0")

    find_package(Boost 1.82.0 REQUIRED)
elseif(APPLE)
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
    set(CMAKE_CXX_STANDARD 14)

    set(BOOST_ROOT "/opt/homebrew/Cellar/boost/1.83.0")
    find_package(Boost 1.83.0 REQUIRED)

    find_package(JPEG REQUIRED)
    find_package(PNG REQUIRED)
elseif(UNIX)
    set(CMAKE_CXX_STANDARD 14)
    
    set(BOOST_ROOT "/usr")
    find_package(Boost 1.70 REQUIRED)

    find_package(JPEG REQUIRED)
    find_package(PNG REQUIRED)

endif()

if(Boost_FOUND)

    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${JPEG_INCLUDE_DIRS})
    include_directories(${PNG_INCLUDE_DIRS})

    add_executable(${PROJECT_NAME} ${SOURCE_FILES})

    target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${JPEG_LIBRARIES}
        ${PNG_LIBRARIES}
    )

else()
    message(FATAL_ERROR "Boost not found. Please set BOOST_ROOT correctly.")
endif()


if(WIN32)

    include_directories(C:\\boost\\boost_1_82_0\\boost)
    link_directories(C:\\boost\\boost_1_82_0\\bin.v2\\libs)
elseif(APPLE)
    include_directories(/opt/homebrew/Cellar/boost/1.83.0/include)
    link_directories(/opt/homebrew/Cellar/boost/1.83.0/lib)

    link_libraries(/opt/homebrew/opt/jpeg/lib/libjpeg.dylib)
    link_libraries(/opt/homebrew/opt/libpng/lib/libpng.dylib)
elseif(UNIX)
    include_directories(/usr/include)
    link_directories(/usr/lib /usr/lib/x86_64-linux-gnu)

    link_libraries(jpeg png)

endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_executable(Build_${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(Build_${PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${PNG_LIBRARIES}
)
