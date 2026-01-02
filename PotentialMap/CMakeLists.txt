cmake_minimum_required(VERSION 3.10)
project(PotentialMap)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_compile_definitions(_USE_MATH_DEFINES)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

# Library source files (without main.cpp)
set(LIB_SOURCE_FILES
        ./src/Colourisers.h
        ./src/Colourisers.cpp
        ./src/Calculators.h
        ./src/Calculators.cpp
        ./src/CalcMacros.h
        ./src/GeneralEE.cpp
        ./src/GeneralEE.h
)

# All source files including main
set(SOURCE_FILES
        ./src/main.cpp
        ${LIB_SOURCE_FILES}
)

if(WIN32)
    set(CMAKE_CXX_STANDARD 17)
    find_package(Boost 1.82.0 REQUIRED)
    find_package(JPEG QUIET)
    find_package(PNG QUIET)
elseif(APPLE)
    set(CMAKE_CXX_STANDARD 14)
    find_package(Boost 1.70 REQUIRED)
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
    if(JPEG_FOUND)
        include_directories(${JPEG_INCLUDE_DIRS})
    endif()
    if(PNG_FOUND)
        include_directories(${PNG_INCLUDE_DIRS})
    endif()

    # Create LIBRARY target (for linking into other projects)
    add_library(${PROJECT_NAME} STATIC ${LIB_SOURCE_FILES})
    target_include_directories(${PROJECT_NAME} PUBLIC ./src)
    target_link_libraries(${PROJECT_NAME}
            PUBLIC
            ${Boost_LIBRARIES}
    )
    if(JPEG_FOUND)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${JPEG_LIBRARIES})
    endif()
    if(PNG_FOUND)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${PNG_LIBRARIES})
    endif()

    # Create EXECUTABLE target (standalone program)
    add_executable(${PROJECT_NAME}_exe ${SOURCE_FILES})
    target_link_libraries(${PROJECT_NAME}_exe ${Boost_LIBRARIES})
    if(JPEG_FOUND)
        target_link_libraries(${PROJECT_NAME}_exe ${JPEG_LIBRARIES})
    endif()
    if(PNG_FOUND)
        target_link_libraries(${PROJECT_NAME}_exe ${PNG_LIBRARIES})
    endif()

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
target_link_libraries(Build_${PROJECT_NAME} ${Boost_LIBRARIES})
if(JPEG_FOUND)
    target_link_libraries(Build_${PROJECT_NAME} ${JPEG_LIBRARIES})
endif()
if(PNG_FOUND)
    target_link_libraries(Build_${PROJECT_NAME} ${PNG_LIBRARIES})
endif()