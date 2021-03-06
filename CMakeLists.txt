cmake_minimum_required(VERSION 3.10.0)

project(Homeland)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/Build/${BUILD_PLATFORM}/${CMAKE_BUILD_TYPE})
set(OUTPUT_DIR_INTERMEDIATE ${OUTPUT_DIR}/Intermediate)
set(OUTPUT_DIR_BINARY       ${OUTPUT_DIR}/Binary)
set(OUTPUT_DIR_LIBRARY      ${OUTPUT_DIR}/Library)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR_BINARY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR_LIBRARY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR_LIBRARY})

add_subdirectory(src/Engine)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOROC ON)
set(CMAKE_AUTOUIC ON)

#find_package(Qt5 COMPONENTS Widgets Core Gui Qml Quick REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

qt5_add_resources(Assets src/Homeland/Assets/Assets.qrc)

set(HomelandSrc
    src/Homeland/Main.cpp
    src/Homeland/Private/HomelandApp.cpp
    src/Homeland/HomelandApp.hpp src/Homeland/HomelandBehaviors.cpp src/Homeland/HomelandBehaviors.hpp)

add_executable(Homeland ${HomelandSrc} ${Assets})

target_link_libraries(Homeland PRIVATE
    Engine)

target_include_directories(Homeland PRIVATE
    src)