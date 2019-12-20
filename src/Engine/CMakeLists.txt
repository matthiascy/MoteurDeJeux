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

set(EnginePhysicsSrc
    ${EngineSrcDir}/Physics/Private/Collider.cpp
    ${EngineSrcDir}/Physics/Private/Constraint.cpp
    ${EngineSrcDir}/Physics/Private/DebugDraw.cpp
    ${EngineSrcDir}/Physics/Private/RigidBody.cpp
    ${EngineSrcDir}/Physics/Private/DebugDraw.cpp
    ${EngineSrcDir}/Physics/Colliders/Private/BoxCollider.cpp
    ${EngineSrcDir}/Physics/Colliders/Private/ConeCollider.cpp
    ${EngineSrcDir}/Physics/Colliders/Private/SphereCollider.cpp
    ${EngineSrcDir}/Physics/Colliders/Private/CapsuleCollider.cpp
    ${EngineSrcDir}/Physics/Colliders/Private/CylinderCollider.cpp
    ${EngineSrcDir}/Physics/Colliders/Private/StaticMeshCollider.cpp
    ${EngineSrcDir}/Physics/Colliders/Private/StaticPlaneCollider.cpp
    ${EngineSrcDir}/Physics/Types.hpp
    ${EngineSrcDir}/Physics/Forward.hpp
    ${EngineSrcDir}/Physics/Collider.hpp
    ${EngineSrcDir}/Physics/RigidBody.hpp
    ${EngineSrcDir}/Physics/Colliders.hpp
    ${EngineSrcDir}/Physics/DebugDraw.hpp
    ${EngineSrcDir}/Physics/Constraint.hpp
    ${EngineSrcDir}/Physics/Colliders/BoxCollider.hpp
    ${EngineSrcDir}/Physics/Colliders/ConeCollider.hpp
    ${EngineSrcDir}/Physics/Colliders/SphereCollider.hpp
    ${EngineSrcDir}/Physics/Colliders/CapsuleCollider.hpp
    ${EngineSrcDir}/Physics/Colliders/CylinderCollider.hpp
    ${EngineSrcDir}/Physics/Colliders/StaticMeshCollider.hpp
    ${EngineSrcDir}/Physics/Colliders/StaticPlaneCollider.hpp)

set(EngineGraphicsSrc
    ${EngineSrcDir}/Graphics/Private/Mesh.cpp
    ${EngineSrcDir}/Graphics/Private/Shader.cpp
    ${EngineSrcDir}/Graphics/Private/Material.cpp
    ${EngineSrcDir}/Graphics/Private/SplashScreen.cpp
    ${EngineSrcDir}/Graphics/Components/Private/Light.cpp
    ${EngineSrcDir}/Graphics/Components/Private/Renderer.cpp
    ${EngineSrcDir}/Graphics/Components/Private/MeshRenderer.cpp
    ${EngineSrcDir}/Graphics/OpenGL/Private/OglOffscreenSurface.cpp
    ${EngineSrcDir}/Graphics/Components/Private/DirectionalLight.cpp
    ${EngineSrcDir}/Graphics/Mesh.hpp
    ${EngineSrcDir}/Graphics/Types.hpp
    ${EngineSrcDir}/Graphics/Shader.hpp
    ${EngineSrcDir}/Graphics/SplashScreen.hpp
    ${EngineSrcDir}/Graphics/OpenGL/OglTypes.hpp
    ${EngineSrcDir}/Graphics/Components/Light.hpp
    ${EngineSrcDir}/Graphics/Material.hpp
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
    ${EngineSrcDir}/Core/Object.hpp
    ${EngineSrcDir}/Core/Utils.hpp)

set(EngineGameFrameworkSrc
    ${EngineSrcDir}/GameFramework/Private/Scene.cpp
    ${EngineSrcDir}/GameFramework/Private/System.cpp
    ${EngineSrcDir}/GameFramework/Private/Engine.cpp
    ${EngineSrcDir}/GameFramework/Private/GameApp.cpp
    ${EngineSrcDir}/GameFramework/Private/Component.cpp
    ${EngineSrcDir}/GameFramework/Private/GameObject.cpp
    ${EngineSrcDir}/GameFramework/Scene.hpp
    ${EngineSrcDir}/GameFramework/Types.hpp
    ${EngineSrcDir}/GameFramework/Engine.hpp
    ${EngineSrcDir}/GameFramework/System.hpp
    ${EngineSrcDir}/GameFramework/GameApp.hpp
    ${EngineSrcDir}/GameFramework/Forward.hpp
    ${EngineSrcDir}/GameFramework/Component.hpp
    ${EngineSrcDir}/GameFramework/GameObject.hpp

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
    ${EngineSrcDir}/GameFramework/Components/Private/Transform.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/Camera.cpp
    ${EngineSrcDir}/GameFramework/Components/Behavior.hpp
    ${EngineSrcDir}/GameFramework/Components/Transform.hpp
    ${EngineSrcDir}/GameFramework/Components/Camera.hpp
    ${EngineSrcDir}/GameFramework/Components/Private/OrthogonalCamera.cpp
    ${EngineSrcDir}/GameFramework/Components/OrthogonalCamera.hpp
    ${EngineSrcDir}/GameFramework/Components/Private/PerspectiveCamera.cpp
    ${EngineSrcDir}/GameFramework/Components/PerspectiveCamera.hpp
    ${EngineSrcDir}/GameFramework/Components.hpp

    ${EngineSrcDir}/GameFramework/Systems.hpp
    ${EngineSrcDir}/GameFramework/Systems/Private/BehaviorSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/BehaviorSystem.hpp
    ${EngineSrcDir}/GameFramework/Systems/Private/InputSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/Private/PhysicsSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/Private/RenderSystem.cpp
    ${EngineSrcDir}/GameFramework/Systems/InputSystem.hpp
    ${EngineSrcDir}/GameFramework/Systems/PhysicsSystem.hpp
    ${EngineSrcDir}/GameFramework/Systems/RenderSystem.hpp

    ${EngineSrcDir}/GameFramework/Managers.hpp
    ${EngineSrcDir}/GameFramework/Managers/Private/ComponentManager.cpp
    ${EngineSrcDir}/GameFramework/Managers/Private/SceneManager.cpp
    ${EngineSrcDir}/GameFramework/Managers/ComponentManager.hpp
    ${EngineSrcDir}/GameFramework/Managers/SceneManager.hpp

    ${EngineSrcDir}/GameFramework/Private/EngineWindow.cpp
    ${EngineSrcDir}/GameFramework/EngineWindow.hpp)

set(EngineSrc
    ${EngineCoreSrc}
    ${EngineGraphicsSrc}
    ${EnginePhysicsSrc}
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
    ${EditorSrcDir}/Vertex.cpp)

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