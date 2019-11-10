cmake_minimum_required(VERSION 3.10.0)

project(MoteurDeJeux)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOROC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCE_DIR src)
set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/Build/${BUILD_PLATFORM}/${CMAKE_BUILD_TYPE})
set(OUTPUT_DIR_INTERMEDIATE ${OUTPUT_DIR}/Intermediate)
set(OUTPUT_DIR_BINARY       ${OUTPUT_DIR}/Binary)
set(OUTPUT_DIR_LIBRARY      ${OUTPUT_DIR}/Library)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR_BINARY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR_LIBRARY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR_LIBRARY})

find_package(Qt5 COMPONENTS Widgets Core Gui Qml Quick REQUIRED)

qt5_add_resources(QRCS resources.qrc)

set(MoteurDeJeuxSrc
    src/main.cpp
    src/EngineViewport.cpp
        src/window.cpp
        src/mainwindow.cpp
        src/GameObject.cpp
        src/GLWidget.cpp
        src/Camera.cpp
        src/Light.cpp
        src/Node.cpp
        src/NameGenerator.cpp
        src/SceneNode.cpp
        src/SceneManager.cpp
        src/Shader.cpp
        src/Mesh.cpp
        src/ResourceManager.cpp
        src/MeshLoader.cpp
        src/Terrain.cpp
    src/EngineViewport.hpp
        src/window.h
        src/mainwindow.h
        src/GameObject.hpp
        src/Camera.hpp
        src/Types.hpp
        src/Light.hpp
        src/Node.hpp
        src/NameGenerator.hpp
        src/SceneNode.hpp
        src/SceneManager.hpp
        src/Shader.hpp
        src/Mesh.hpp
        src/ResourceManager.hpp
        src/MeshLoader.hpp
        src/Terrain.hpp
        src/GLWidget.hpp src/EngineRenderer.cpp src/EngineRenderer.hpp
    src/editor/AssetPreviewer.cpp
    src/editor/AssetPreviewer.hpp
    src/editor/AssetsBrowser.cpp
    src/editor/AssetsBrowser.hpp
    src/editor/CommandLine.cpp
    src/editor/CommandLine.hpp
    src/editor/Explorer.cpp
    src/editor/Explorer.hpp
    src/editor/LevelEditorMainWindow.cpp
    src/editor/LevelEditorMainWindow.hpp
    src/editor/PropertyEditor.cpp
    src/editor/PropertyEditor.hpp
    src/editor/SimpleOutput.cpp
    src/editor/SimpleOutput.hpp
    src/editor/Viewport.hpp
    src/editor/Viewport.cpp
    src/editor/Vertex.hpp
    src/editor/Vertex.cpp src/SolarSystem.cpp src/SolarSystem.hpp)

include(cmake/assimp.cmake)

add_executable(MoteurDeJeux ${MoteurDeJeuxSrc} ${QRCS})

#if(WIN32)
#    if(MSVC)
#        set_target_properties(MoteurDeJeux PROPERTIES
#            WIN32_EXECUTABLE YES
#            LINK_FLAGS "/ENTRY:mainCRTStartup")
#        set_target_properties(MoteurDeJeux PROPERTIES
#            WIN32_EXECUTABLE YES
#            LINK_FLAGS "/ENTRY:mainCRTStartup")
#    else()
#        message(SEND_ERROR "You are using an unsupported Windows compiler ! (Not MSVC)")
#    endif(MSVC)
#endif(WIN32)

target_link_libraries(MoteurDeJeux PRIVATE
    Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml
    assimp)
target_include_directories(MoteurDeJeux
    PRIVATE src/assimp/assimp/include)