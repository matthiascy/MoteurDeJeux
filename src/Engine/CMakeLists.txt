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

# QtUiTools used for load .ui file dynamically UiTools
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

qt5_add_resources(QRcs ${EngineSrcDir}/resources.qrc)

set(EngineGraphicsSrc
    ${EngineSrcDir}/Graphics/Private/Mesh.cpp
    ${EngineSrcDir}/Graphics/Private/Shader.cpp
    ${EngineSrcDir}/Graphics/Private/MaterialData.cpp
    ${EngineSrcDir}/Graphics/Private/SplashScreen.cpp
    ${EngineSrcDir}/Graphics/Components/Private/Light.cpp
    ${EngineSrcDir}/Graphics/Components/Private/Renderer.inl
    ${EngineSrcDir}/Graphics/Components/Private/Material.cpp
    ${EngineSrcDir}/Graphics/Components/Private/MeshRenderer.cpp
    ${EngineSrcDir}/Graphics/OpenGL/Private/OglOffscreenSurface.cpp
    ${EngineSrcDir}/Graphics/Components/Private/DirectionalLight.cpp
    ${EngineSrcDir}/Graphics/Mesh.hpp
    ${EngineSrcDir}/Graphics/Types.hpp
    ${EngineSrcDir}/Graphics/Shader.hpp
    ${EngineSrcDir}/Graphics/SplashScreen.hpp
    ${EngineSrcDir}/Graphics/MaterialData.hpp
    ${EngineSrcDir}/Graphics/OpenGL/OglTypes.hpp
    ${EngineSrcDir}/Graphics/Components/Light.hpp
    ${EngineSrcDir}/Graphics/Components/Material.hpp
    ${EngineSrcDir}/Graphics/Components/Renderer.hpp
    ${EngineSrcDir}/Graphics/Components/MeshRenderer.hpp
    ${EngineSrcDir}/Graphics/OpenGL/OglOffscreenSurface.hpp
    ${EngineSrcDir}/Graphics/Components/DirectionalLight.hpp)

set(EngineCoreSrc
    ${EngineSrcDir}/Core/Math/Private/Math.cpp
    ${EngineSrcDir}/Core/Math/Math.hpp
    ${EngineSrcDir}/Core/Math/MathTypes.hpp
    ${EngineSrcDir}/Core/Private/Object.cpp
    ${EngineSrcDir}/Core/BasicTypes.hpp
    ${EngineSrcDir}/Core/Container.hpp
    ${EngineSrcDir}/Core/Core.hpp
    ${EngineSrcDir}/Core/Handle.hpp
    ${EngineSrcDir}/Core/Memory.hpp
    ${EngineSrcDir}/Core/Object.hpp)

set(EngineGameFrameworkSrc
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
    ${EngineSrcDir}/GameFramework/Components/Private/Collider.inl
    ${EngineSrcDir}/GameFramework/Components/Private/Transform.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/Camera.inl
    ${EngineSrcDir}/GameFramework/Components/Behavior.hpp
    ${EngineSrcDir}/GameFramework/Components/BoxCollider.hpp
    ${EngineSrcDir}/GameFramework/Components/Collider.hpp
    ${EngineSrcDir}/GameFramework/Components/Transform.hpp
    ${EngineSrcDir}/GameFramework/Components/Camera.hpp

    ${EngineSrcDir}/GameFramework/Systems/Private/InputSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/Private/PhysicsSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/Private/RenderSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/InputSystem.hpp
    ${EngineSrcDir}/GameFramework/Systems/PhysicsSystem.hpp
    ${EngineSrcDir}/GameFramework/Systems/RenderSystem.hpp

    ${EngineSrcDir}/GameFramework/Private/Component.inl
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

    GameFramework/Components/Private/OrthogonalCamera.cpp
    GameFramework/Components/OrthogonalCamera.hpp
    GameFramework/Components/Private/PerspectiveCamera.cpp
    GameFramework/Components/PerspectiveCamera.hpp)

set(EngineSrc
    ${EngineCoreSrc}
    ${EngineGraphicsSrc}
    ${EngineGameFrameworkSrc}

    ${EditorSrcDir}/EditorCamera.hpp
    ${EditorSrcDir}/EditorCamera.cpp
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
    ${EditorSrcDir}/Vertex.cpp
    ${EngineSrcDir}/GameFramework/Private/EngineWindow.cpp
    ${EngineSrcDir}/GameFramework/EngineWindow.hpp
    GameFramework/Components.hpp
    GameFramework/Managers/Private/SceneManager.cpp
    GameFramework/Managers/SceneManager.hpp
    GameFramework/Managers/Private/ComponentManager.cpp
    GameFramework/Managers/ComponentManager.hpp
    GameFramework/Types.hpp
    GameFramework/TypeID.hpp
    GameFramework/TypeID.hpp
    GameFramework/Private/AbstractComponent.cpp
    GameFramework/AbstractComponent.hpp
    GameFramework/Systems/Private/BehaviorSystem.cpp
    GameFramework/Systems/BehaviorSystem.hpp)

add_library(Engine ${EngineSrc} ${QRcs})

set_target_properties(Engine PROPERTIES
    LINKER_LANGUAGE CXX
    ARCHIVE_OUTPUT_DIRECTORY ${EngineOutputDir}
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR_BINARY}
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR_BINARY})

# Qt5::UiTools
target_link_libraries(Engine PUBLIC
    Qt5::Widgets Qt5::Core Qt5::Gui
    assimp
    BulletDynamics BulletCollision BulletSoftBody
    LinearMath Bullet3Common Bullet3Dynamics
    Bullet3Collision Bullet3Geometry)

target_include_directories(Engine PUBLIC
    ${EngineSrcDir}
    ${EngineSrcDir}/Thirdparty/assimp/assimp/include
    ${EngineSrcDir}/Thirdparty/bullet3/src)