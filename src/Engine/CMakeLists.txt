cmake_minimum_required(VERSION 3.10.0)

project(Engine)

set(EngineSrcDir ${CMAKE_CURRENT_SOURCE_DIR})
set(EditorSrcDir ${EngineSrcDir}/Editor)
set(EngineOutputDir ${OUTPUT_DIR_LIBRARY})

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
include(Thirdparty/zlib.cmake)
include(Thirdparty/assimp.cmake)
include(Thirdparty/bullet3.cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOROC ON)
set(CMAKE_AUTOUIC_ON)

find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

qt5_add_resources(QRcs ${EngineSrcDir}/resources.qrc)

set(EngineSrc
    ${EngineSrcDir}/Core/Math/Math.hpp
    ${EngineSrcDir}/Core/Math/MathTypes.hpp
    ${EngineSrcDir}/Core/Private/Object.cpp
    ${EngineSrcDir}/Core/BasicTypes.hpp
    ${EngineSrcDir}/Core/Container.hpp
    ${EngineSrcDir}/Core/Core.hpp
    ${EngineSrcDir}/Core/Handle.hpp
    ${EngineSrcDir}/Core/Memory.hpp
    ${EngineSrcDir}/Core/Object.hpp

    ${EngineSrcDir}/GameFramework/Assets/Private/Asset.cpp
    ${EngineSrcDir}/GameFramework/Assets/Private/AssetManager.cpp
    ${EngineSrcDir}/GameFramework/Assets/Private/AssetStorage.cpp
    ${EngineSrcDir}/GameFramework/Assets/Private/MeshLoader.cpp
    ${EngineSrcDir}/GameFramework/Assets/Asset.hpp
    ${EngineSrcDir}/GameFramework/Assets/AssetManager.hpp
    ${EngineSrcDir}/GameFramework/Assets/AssetStorage.hpp
    ${EngineSrcDir}/GameFramework/Assets/AssetsTypes.hpp
    ${EngineSrcDir}/GameFramework/Assets/MeshLoader.hpp

    ${EngineSrcDir}/GameFramework/Components/Private/Behavior.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/BoxCollider.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/Collider.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/DirectionalLight.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/Light.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/MeshRenderer.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/Renderer.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/Transform.cpp
    ${EngineSrcDir}/GameFramework/Components/Behavior.hpp
    ${EngineSrcDir}/GameFramework/Components/BoxCollider.hpp
    ${EngineSrcDir}/GameFramework/Components/Collider.hpp
    ${EngineSrcDir}/GameFramework/Components/DirectionalLight.hpp
    ${EngineSrcDir}/GameFramework/Components/Light.hpp
    ${EngineSrcDir}/GameFramework/Components/MeshRenderer.hpp
    ${EngineSrcDir}/GameFramework/Components/Renderer.hpp
    ${EngineSrcDir}/GameFramework/Components/Transform.hpp

    ${EngineSrcDir}/GameFramework/Systems/Private/InputSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/Private/PhysicsSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/Private/RenderSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/InputSystem.hpp
    ${EngineSrcDir}/GameFramework/Systems/PhysicsSystem.hpp
    ${EngineSrcDir}/GameFramework/Systems/RenderSystem.hpp

    ${EngineSrcDir}/GameFramework/Private/Component.cpp
    ${EngineSrcDir}/GameFramework/Private/Engine.cpp
    ${EngineSrcDir}/GameFramework/Private/GameApp.cpp
    ${EngineSrcDir}/GameFramework/Private/GameObject.cpp
    ${EngineSrcDir}/GameFramework/Private/Scene.cpp
    ${EngineSrcDir}/GameFramework/Private/System.cpp
    ${EngineSrcDir}/GameFramework/Component.hpp
    ${EngineSrcDir}/GameFramework/Engine.hpp
    ${EngineSrcDir}/GameFramework/Forward.hpp
    ${EngineSrcDir}/GameFramework/GameApp.hpp
    ${EngineSrcDir}/GameFramework/GameFrameworkTypes.hpp
    ${EngineSrcDir}/GameFramework/GameObject.hpp
    ${EngineSrcDir}/GameFramework/Scene.hpp
    ${EngineSrcDir}/GameFramework/System.hpp

    ${EngineSrcDir}/Graphics/Private/Mesh.cpp
    #${EngineSrcDir}/Core/Graphics/Private/OpenGLError.cpp
    ${EngineSrcDir}/Graphics/Private/OpenGLWindow.cpp
    ${EngineSrcDir}/Graphics/Private/Shader.cpp
    ${EngineSrcDir}/Graphics/GraphicsTypes.hpp
    ${EngineSrcDir}/Graphics/Mesh.hpp
    #${EngineSrcDir}/Core/Graphics/OpenGLError.hpp
    ${EngineSrcDir}/Graphics/OpenGLWindow.hpp
    ${EngineSrcDir}/Graphics/Shader.hpp

    ${EngineSrcDir}/Camera.hpp
    ${EngineSrcDir}/Camera.cpp
    #${EngineSrcDir}/GLWidget.hpp
    #${EngineSrcDir}/GLWidget.cpp
    #${EngineSrcDir}/main.cpp
    ${EngineSrcDir}/NameGenerator.hpp
    ${EngineSrcDir}/NameGenerator.cpp
    #${EngineSrcDir}/Terrain.hpp
    #${EngineSrcDir}/Terrain.cpp
    ${EngineSrcDir}/Types.hpp

    ${EditorSrcDir}/AssetPreviewer.cpp
    ${EditorSrcDir}/AssetPreviewer.hpp
    ${EditorSrcDir}/AssetsBrowser.cpp
    ${EditorSrcDir}/AssetsBrowser.hpp
    ${EditorSrcDir}/CommandLine.cpp
    ${EditorSrcDir}/CommandLine.hpp
    ${EditorSrcDir}/Explorer.cpp
    ${EditorSrcDir}/Explorer.hpp
    ${EditorSrcDir}/EditorMainWindow.cpp
    ${EditorSrcDir}/EditorMainWindow.hpp
    ${EditorSrcDir}/PropertyEditor.cpp
    ${EditorSrcDir}/PropertyEditor.hpp
    ${EditorSrcDir}/SimpleOutput.cpp
    ${EditorSrcDir}/SimpleOutput.hpp
    ${EditorSrcDir}/Viewport.hpp
    ${EditorSrcDir}/Viewport.cpp
    ${EditorSrcDir}/Vertex.hpp
    ${EditorSrcDir}/Vertex.cpp Graphics/Private/OglOffscreenSurface.cpp Graphics/OglOffscreenSurface.hpp Graphics/AbstractWindow.hpp)

add_library(Engine ${EngineSrc} ${QRcs})

set_target_properties(Engine PROPERTIES
    LINKER_LANGUAGE CXX
    ARCHIVE_OUTPUT_DIRECTORY ${EngineOutputDir}
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR_BINARY}
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR_BINARY})

target_link_libraries(Engine PUBLIC
    Qt5::Widgets Qt5::Core Qt5::Gui
    assimp Bullet3Common BulletSoftBody Bullet3Dynamics
    Bullet3Collision LinearMath Bullet3Geometry
    BulletDynamics BulletCollision)

target_include_directories(Engine PUBLIC
    ${EngineSrcDir}
    ${EngineSrcDir}/Thirdparty/assimp/assimp/include
    ${EngineSrcDir}/Thirdparty/bullet3/src)