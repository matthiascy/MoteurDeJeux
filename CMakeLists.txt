cmake_minimum_required(VERSION 3.10.0)

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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOROC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core Gui Qml Quick REQUIRED)

qt5_add_resources(QRCS resources.qrc)

set(GameFrameworkSrc
    src/Engine/GameFramework/Private/GameObject.cpp
    src/Engine/GameFramework/Private/Scene.cpp
    src/Engine/GameFramework/Components/Private/Transform.cpp
    src/Engine/GameFramework/Private/Component.cpp
    src/Engine/GameFramework/Private/System.cpp
    src/Engine/GameFramework/Components/Private/Collider.cpp
    src/Engine/GameFramework/Components/Private/BoxCollider.cpp
    src/Engine/GameFramework/GameFrameworkTypes.hpp
    src/Engine/GameFramework/GameObject.hpp
    src/Engine/GameFramework/Scene.hpp
    src/Engine/GameFramework/Component.hpp
    src/Engine/GameFramework/Components/Transform.hpp
    src/Engine/GameFramework/Components/Collider.hpp
    src/Engine/GameFramework/Components/BoxCollider.hpp
    src/Engine/GameFramework/System.hpp
    src/Engine/GameFramework/Subsystems/Private/PhysicsSystem.cpp
    src/Engine/GameFramework/Subsystems/PhysicsSystem.hpp
    src/Engine/GameFramework/Components/Private/Behavior.cpp
    src/Engine/GameFramework/Components/Behavior.hpp
    src/Engine/GameFramework/Engine.hpp
    src/Engine/GameFramework/Private/Engine.cpp
    src/Engine/GameFramework/Private/GameApp.cpp
    src/Engine/GameFramework/GameApp.hpp)

set(EditorSrc
    src/editor/AssetPreviewer.cpp
    src/editor/AssetPreviewer.hpp
    src/editor/AssetsBrowser.cpp
    src/editor/AssetsBrowser.hpp
    src/editor/CommandLine.cpp
    src/editor/CommandLine.hpp
    src/editor/Explorer.cpp
    src/editor/Explorer.hpp
    src/Editor/EditorMainWindow.cpp
    src/Editor/EditorMainWindow.hpp
    src/editor/PropertyEditor.cpp
    src/editor/PropertyEditor.hpp
    src/editor/SimpleOutput.cpp
    src/editor/SimpleOutput.hpp
    src/editor/Viewport.hpp
    src/editor/Viewport.cpp
    src/editor/Vertex.hpp
    src/editor/Vertex.cpp)

set(HomelandSrc
    src/Homeland/Private/HomelandApp.cpp
    src/Homeland/Main.cpp
    src/Homeland/HomelandApp.hpp
)

set(MoteurDeJeuxSrc
    src/EngineViewport.cpp
    src/mainwindow.cpp
    #src/GLWidget.cpp
    src/Camera.cpp
    src/NameGenerator.cpp
    src/Engine/Graphics/Private/Mesh.cpp
    src/Engine/Assets/Private/MeshLoader.cpp
    src/Terrain.cpp
    src/EngineViewport.hpp
    src/mainwindow.h
    src/Camera.hpp
    src/Types.hpp
    src/NameGenerator.hpp
    src/Engine/Graphics/Mesh.hpp
    src/to-delete/ResourceManager.hpp
    src/Engine/Assets/MeshLoader.hpp
    src/Terrain.hpp
    #src/GLWidget.hpp
    src/EngineRenderer.cpp src/EngineRenderer.hpp
    src/SolarSystem.cpp
    src/SolarSystem.hpp
    src/Engine/Assets/Private/AssetStorage.cpp src/Engine/assets/AssetStorage.hpp
    src/Engine/Core/CoreTypes.hpp
    src/Engine/Core/Core.hpp
    src/Engine/Core/Math/MathTypes.hpp
    src/Engine/Core/Math/Math.hpp
    src/Engine/Core/BasicTypes.hpp
    src/Engine/Core/ContainerTypes.hpp src/Engine/Core/MemoryTypes.hpp src/Engine/Core/Memory.hpp
    src/Engine/Core/Object.hpp
    src/Engine/Core/Private/Object.cpp
    src/Engine/Core/HandleType.hpp src/Engine/Core/Handle.hpp
    ${GameFrameworkSrc}
    ${EditorSrc} src/Engine/GameFramework/Components/Private/MeshRenderable.cpp src/Engine/GameFramework/Components/MeshRenderable.hpp src/Engine/GameFramework/Components/Private/Renderable.cpp src/Engine/GameFramework/Components/Renderable.hpp src/Engine/GameFramework/Subsystems/Private/RendererSystem.cpp src/Engine/GameFramework/Subsystems/RendererSystem.hpp src/Engine/Assets/Private/Asset.cpp src/Engine/Assets/Asset.hpp src/Engine/Assets/Private/AssetManager.cpp src/Engine/Assets/AssetManager.hpp src/Engine/Core/QtTypes.hpp src/Engine/Assets/AssetsTypes.hpp src/Engine/Core/Container.hpp
    src/Engine/Graphics/Private/OpenGLWindow.cpp
    src/Engine/Graphics/OpenGLWindow.hpp
    src/Engine/Graphics/Window.hpp
    ${HomelandSrc})

add_executable(MoteurDeJeux ${MoteurDeJeuxSrc} ${QRCS})

target_link_libraries(MoteurDeJeux PRIVATE
    Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml
    assimp Bullet3Common BulletSoftBody Bullet3Dynamics
    Bullet3Collision LinearMath Bullet3Geometry
    BulletDynamics BulletCollision)
target_include_directories(MoteurDeJeux PRIVATE
    ${SOURCE_DIR}
    src/Engine/Thirdparty/assimp/assimp/include
    src/Engine/Thirdparty/bullet3/src/)
target_compile_definitions(MoteurDeJeux PRIVATE
    ${BULLET_DEFINITIONS})

add_executable(Bullet3Test src/Bullet3Test.cpp)
target_compile_definitions(Bullet3Test PRIVATE
    ${BULLET_DEFINITIONS})
target_link_libraries(Bullet3Test PRIVATE
    Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Quick Qt5::Qml
    Bullet3Common BulletSoftBody Bullet3Dynamics
    Bullet3Collision LinearMath Bullet3Geometry
    BulletDynamics BulletCollision)
target_include_directories(Bullet3Test PRIVATE
    src/Engine/Thirdparty/bullet3/src)