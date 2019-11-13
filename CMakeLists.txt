cmake_minimum_required(VERSION 3.15.0)

project(MoteurDeJeux)

set(SOURCE_DIR src)
set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/Build/${BUILD_PLATFORM}/${CMAKE_BUILD_TYPE})
set(OUTPUT_DIR_INTERMEDIATE ${OUTPUT_DIR}/Intermediate)
set(OUTPUT_DIR_BINARY       ${OUTPUT_DIR}/Binary)
set(OUTPUT_DIR_LIBRARY      ${OUTPUT_DIR}/Library)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR_BINARY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR_LIBRARY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR_LIBRARY})

# because of bullet3
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(cmake/zlib.cmake)
include(cmake/assimp.cmake)
include(cmake/bullet3.cmake)

add_executable(Bullet3Test src/Bullet3Test.cpp)
target_compile_definitions(Bullet3Test PRIVATE
    ${BULLET_DEFINITIONS})
target_link_directories(Bullet3Test PRIVATE ${BULLET_LIBRARY_DIRS})
target_link_libraries(Bullet3Test PRIVATE
    Bullet3Common BulletSoftBody Bullet3Dynamics
    Bullet3Collision LinearMath Bullet3Geometry
    BulletDynamics BulletCollision)
target_include_directories(Bullet3Test PRIVATE
    src/thirdparty/bullet3/src)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOROC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core Gui Qml Quick REQUIRED)

qt5_add_resources(QRCS resources.qrc)

set(MoteurDeJeuxSrc
    src/main.cpp
    src/EngineViewport.cpp
    #src/Window.cpp
    src/mainwindow.cpp
    src/GameObject.cpp
    src/GLWidget.cpp
    src/Camera.cpp
    src/Light.cpp
    #src/Node.cpp
    src/NameGenerator.cpp
    #src/SceneNode.cpp
    src/Scene.cpp
    #src/Shader.cpp
    src/Mesh.cpp
    src/ResourceManager.cpp
    src/MeshLoader.cpp
    src/Terrain.cpp
    src/EngineViewport.hpp
    #src/Window.hpp
    src/mainwindow.h
    src/GameObject.hpp
    src/Camera.hpp
    src/Types.hpp
    src/Light.hpp
    #src/Node.hpp
    src/NameGenerator.hpp
    #src/SceneNode.hpp
    src/Scene.hpp
    #src/Shader.hpp
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
    src/editor/Vertex.cpp
    src/SolarSystem.cpp
    src/SolarSystem.hpp
    #src/graphics/OpenGLError.cpp
    #src/graphics/OpenGLError.hpp
    #src/Input.cpp src/Input.hpp
    src/subsystem/PhysicsSystem.cpp
    src/subsystem/PhysicsSystem.hpp
    src/GameFramework/Transform.cpp
    src/GameFramework/Transform.hpp
    src/physics/Collider.cpp src/physics/Collider.hpp src/physics/BoxCollider.cpp src/physics/BoxCollider.hpp src/assets/AssetStorage.cpp src/assets/AssetStorage.hpp src/GameFramework/GameFrameworkTypes.hpp src/Core/CoreTypes.hpp src/Core/Core.hpp src/Core/Math/MathTypes.hpp src/Core/Math/Math.hpp src/Core/BasicTypes.hpp src/Core/ContainerTypes.hpp src/Core/MemoryTypes.hpp src/Core/Memory.hpp src/Core/Object.hpp src/Core/Private/Object.cpp)

add_executable(MoteurDeJeux ${MoteurDeJeuxSrc} ${QRCS})

target_link_libraries(MoteurDeJeux PRIVATE
    Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml
    assimp Bullet3Common BulletSoftBody Bullet3Dynamics
    Bullet3Collision LinearMath Bullet3Geometry
    BulletDynamics BulletCollision)
target_include_directories(MoteurDeJeux PRIVATE
    ${SOURCE_DIR}
    src/thirdparty/assimp/assimp/include
    src/thirdparty/bullet3/src/)
target_compile_definitions(MoteurDeJeux PRIVATE
    ${BULLET_DEFINITIONS})