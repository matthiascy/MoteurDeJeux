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

set(EnginePhysicsSrc
    ${EngineSrcDir}/Physics/Private/Collider.cpp
    ${EngineSrcDir}/Physics/Private/Constraint.cpp
    ${EngineSrcDir}/Physics/Private/DebugDrawer.cpp
    ${EngineSrcDir}/Physics/Private/RigidBody.cpp
    ${EngineSrcDir}/Physics/Private/Colliders/BoxCollider.cpp
    ${EngineSrcDir}/Physics/Private/Colliders/ConeCollider.cpp
    ${EngineSrcDir}/Physics/Private/Colliders/SphereCollider.cpp
    ${EngineSrcDir}/Physics/Private/Colliders/CapsuleCollider.cpp
    ${EngineSrcDir}/Physics/Private/Colliders/CylinderCollider.cpp
    ${EngineSrcDir}/Physics/Private/Colliders/StaticMeshCollider.cpp
    ${EngineSrcDir}/Physics/Private/Colliders/StaticPlaneCollider.cpp
    ${EngineSrcDir}/Physics/Public/Types.hpp
    ${EngineSrcDir}/Physics/Public/Forward.hpp
    ${EngineSrcDir}/Physics/Public/Collider.hpp
    ${EngineSrcDir}/Physics/Public/RigidBody.hpp
    ${EngineSrcDir}/Physics/Public/Colliders.hpp
    ${EngineSrcDir}/Physics/Public/Constraint.hpp
    ${EngineSrcDir}/Physics/Public/DebugDrawer.hpp
    ${EngineSrcDir}/Physics/Public/Colliders/BoxCollider.hpp
    ${EngineSrcDir}/Physics/Public/Colliders/ConeCollider.hpp
    ${EngineSrcDir}/Physics/Public/Colliders/SphereCollider.hpp
    ${EngineSrcDir}/Physics/Public/Colliders/CapsuleCollider.hpp
    ${EngineSrcDir}/Physics/Public/Colliders/CylinderCollider.hpp
    ${EngineSrcDir}/Physics/Public/Colliders/StaticMeshCollider.hpp
    ${EngineSrcDir}/Physics/Public/Colliders/StaticPlaneCollider.hpp)

set(EngineGraphicsSrc
    ${EngineSrcDir}/Graphics/Private/Mesh.cpp
    ${EngineSrcDir}/Graphics/Private/Shader.cpp
    ${EngineSrcDir}/Graphics/Private/Material.cpp
    ${EngineSrcDir}/Graphics/Private/SplashScreen.cpp
    ${EngineSrcDir}/Graphics/Private/Light.cpp
    ${EngineSrcDir}/Graphics/Private/Renderer.cpp
    ${EngineSrcDir}/Graphics/Private/MeshRenderer.cpp
    ${EngineSrcDir}/Graphics/Private/OpenGL/OglOffscreenSurface.cpp
    ${EngineSrcDir}/Graphics/Private/DirectionalLight.cpp
    ${EngineSrcDir}/Graphics/Public/Mesh.hpp
    ${EngineSrcDir}/Graphics/Public/Types.hpp
    ${EngineSrcDir}/Graphics/Public/Shader.hpp
    ${EngineSrcDir}/Graphics/Public/SplashScreen.hpp
    ${EngineSrcDir}/Graphics/Public/OpenGL/OglTypes.hpp
    ${EngineSrcDir}/Graphics/Public/Light.hpp
    ${EngineSrcDir}/Graphics/Public/Material.hpp
    ${EngineSrcDir}/Graphics/Public/MeshRenderer.hpp
    ${EngineSrcDir}/Graphics/Public/OpenGL/OglOffscreenSurface.hpp
    ${EngineSrcDir}/Graphics/Public/DirectionalLight.hpp)

set(EngineCoreSrc
    ${EngineSrcDir}/Core/Private/Object.cpp
    ${EngineSrcDir}/Core/Private/Math/Math.cpp
    ${EngineSrcDir}/Core/Public/Core.hpp
    ${EngineSrcDir}/Core/Public/Utils.hpp
    ${EngineSrcDir}/Core/Public/Handle.hpp
    ${EngineSrcDir}/Core/Public/Memory.hpp
    ${EngineSrcDir}/Core/Public/Object.hpp
    ${EngineSrcDir}/Core/Public/Math/Math.hpp
    ${EngineSrcDir}/Core/Public/Container.hpp
    ${EngineSrcDir}/Core/Public/BasicTypes.hpp
    ${EngineSrcDir}/Core/Public/Math/MathTypes.hpp)

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

    ${EngineSrcDir}/GameFramework/Assets/Asset.hpp
    ${EngineSrcDir}/GameFramework/Assets/Private/Asset.cpp
    ${EngineSrcDir}/GameFramework/Assets/Private/MeshLoader.cpp
    ${EngineSrcDir}/GameFramework/Assets/Private/AssetManager.cpp
    ${EngineSrcDir}/GameFramework/Assets/Private/AssetStorage.cpp
    ${EngineSrcDir}/GameFramework/Assets/MeshLoader.hpp
    ${EngineSrcDir}/GameFramework/Assets/AssetsTypes.hpp
    ${EngineSrcDir}/GameFramework/Assets/AssetManager.hpp
    ${EngineSrcDir}/GameFramework/Assets/AssetStorage.hpp

    ${EngineSrcDir}/GameFramework/Components/Private/Camera.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/Behavior.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/Transform.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/OrthogonalCamera.cpp
    ${EngineSrcDir}/GameFramework/Components/Private/PerspectiveCamera.cpp
    ${EngineSrcDir}/GameFramework/Components.hpp
    ${EngineSrcDir}/GameFramework/Components/Camera.hpp
    ${EngineSrcDir}/GameFramework/Components/Behavior.hpp
    ${EngineSrcDir}/GameFramework/Components/Transform.hpp
    ${EngineSrcDir}/GameFramework/Components/OrthogonalCamera.hpp
    ${EngineSrcDir}/GameFramework/Components/PerspectiveCamera.hpp

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
    ${EditorSrcDir}/Vertex.cpp Physics/Private/PhysicsWorld.cpp Physics/Public/PhysicsWorld.hpp Physics/Public/BulletForward.hpp Physics/Private/CollisionShape.cpp Physics/Public/CollisionShape.hpp)

qt5_add_resources(EngineSrc ${EngineSrcDir}/rscs.qrc ${EngineSrcDir}/Resources/Resources.qrc)

add_library(Engine ${EngineSrc})

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