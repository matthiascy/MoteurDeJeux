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
    ${EngineSrcDir}/Physics/Private/RigidBody.cpp
    ${EngineSrcDir}/Physics/Private/Constraint.cpp
    ${EngineSrcDir}/Physics/Private/PhysicsWorld.cpp
    ${EngineSrcDir}/Physics/Private/CollisionShape.cpp
    ${EngineSrcDir}/Physics/Private/PhysicsDebugDraw.cpp
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
    ${EngineSrcDir}/Physics/Public/PhysicsWorld.hpp
    ${EngineSrcDir}/Physics/Public/CollisionShape.hpp
    ${EngineSrcDir}/Physics/Public/PhysicsDebugDraw.hpp
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
    ${EngineSrcDir}/Graphics/Private/Light.cpp
    ${EngineSrcDir}/Graphics/Private/Renderer.cpp
    ${EngineSrcDir}/Graphics/Private/MeshRenderer.cpp
    ${EngineSrcDir}/Graphics/Private/SplashScreen.cpp
    ${EngineSrcDir}/Graphics/Private/RenderSystem.cpp
    ${EngineSrcDir}/Graphics/Private/DirectionalLight.cpp
    ${EngineSrcDir}/Graphics/Private/OpenGL/OglOffscreenSurface.cpp
    ${EngineSrcDir}/Graphics/Public/Mesh.hpp
    ${EngineSrcDir}/Graphics/Public/Types.hpp
    ${EngineSrcDir}/Graphics/Public/Shader.hpp
    ${EngineSrcDir}/Graphics/Public/Light.hpp
    ${EngineSrcDir}/Graphics/Public/Material.hpp
    ${EngineSrcDir}/Graphics/Public/RenderSystem.hpp
    ${EngineSrcDir}/Graphics/Public/MeshRenderer.hpp
    ${EngineSrcDir}/Graphics/Public/SplashScreen.hpp
    ${EngineSrcDir}/Graphics/Public/OpenGL/OglTypes.hpp
    ${EngineSrcDir}/Graphics/Public/DirectionalLight.hpp
    ${EngineSrcDir}/Graphics/Public/OpenGL/OglOffscreenSurface.hpp)

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
    ${EngineSrcDir}/GameFramework/Private/ECS/System.cpp
    ${EngineSrcDir}/GameFramework/Private/Engine.cpp
    ${EngineSrcDir}/GameFramework/Private/GameApp.cpp
    ${EngineSrcDir}/GameFramework/Private/ECS/Component.cpp
    ${EngineSrcDir}/GameFramework/Private/ECS/GameObject.cpp
    ${EngineSrcDir}/GameFramework/Public/Scene.hpp
    ${EngineSrcDir}/GameFramework/Public/Types.hpp
    ${EngineSrcDir}/GameFramework/Public/Engine.hpp
    ${EngineSrcDir}/GameFramework/Public/ECS/System.hpp
    ${EngineSrcDir}/GameFramework/Public/GameApp.hpp
    ${EngineSrcDir}/GameFramework/Public/Forward.hpp
    ${EngineSrcDir}/GameFramework/Public/ECS/Component.hpp
    ${EngineSrcDir}/GameFramework/Public/ECS/GameObject.hpp

    ${EngineSrcDir}/GameFramework/Private/Managers/AssetManager.cpp
    ${EngineSrcDir}/GameFramework/Public/Managers/AssetManager.hpp

    ${EngineSrcDir}/GameFramework/Private/Components/Camera.cpp
    ${EngineSrcDir}/GameFramework/Private/Components/Behavior.cpp
    ${EngineSrcDir}/GameFramework/Private/Components/Transform.cpp
    ${EngineSrcDir}/GameFramework/Private/Components/OrthogonalCamera.cpp
    ${EngineSrcDir}/GameFramework/Private/Components/PerspectiveCamera.cpp
    ${EngineSrcDir}/GameFramework/Public/Components.hpp
    ${EngineSrcDir}/GameFramework/Public/Components/Camera.hpp
    ${EngineSrcDir}/GameFramework/Public/Components/Behavior.hpp
    ${EngineSrcDir}/GameFramework/Public/Components/Transform.hpp
    ${EngineSrcDir}/GameFramework/Public/Components/OrthogonalCamera.hpp
    ${EngineSrcDir}/GameFramework/Public/Components/PerspectiveCamera.hpp

    ${EngineSrcDir}/GameFramework/Public/Systems.hpp
    ${EngineSrcDir}/GameFramework/Private/Systems/BehaviorSystem.cpp
    ${EngineSrcDir}/GameFramework/Public/Systems/BehaviorSystem.hpp
    ${EngineSrcDir}/GameFramework/Private/Systems/InputSystem.cpp
    ${EngineSrcDir}/GameFramework/Private/Systems/PhysicsSystem.cpp
    ${EngineSrcDir}/GameFramework/Public/Systems/InputSystem.hpp
    ${EngineSrcDir}/GameFramework/Public/Systems/PhysicsSystem.hpp

    ${EngineSrcDir}/GameFramework/Public/Managers.hpp
    ${EngineSrcDir}/GameFramework/Private/Managers/ComponentManager.cpp
    ${EngineSrcDir}/GameFramework/Private/Managers/SceneManager.cpp
    ${EngineSrcDir}/GameFramework/Public/Managers/ComponentManager.hpp
    ${EngineSrcDir}/GameFramework/Public/Managers/SceneManager.hpp

    ${EngineSrcDir}/GameFramework/Private/EngineWindow.cpp
    ${EngineSrcDir}/GameFramework/Public/EngineWindow.hpp)

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
    ${EditorSrcDir}/Vertex.cpp Core/Private/Math/BoundingBox.cpp Core/Public/Math/BoundingBox.hpp Physics/Public/BulletTypes.hpp GameFramework/Public/ECS/Family.hpp Core/Public/NameGenerator.cpp Core/Public/NameGenerator.hpp Graphics/Private/Model.cpp Graphics/Public/Model.hpp Graphics/Private/PointLight.cpp Graphics/Public/PointLight.hpp Graphics/Private/SpotLight.cpp Graphics/Public/SpotLight.hpp Graphics/Public/Forward.hpp Graphics/Public/Texture.hpp Graphics/Private/Texture.cpp Graphics/Public/Texture.hpp Graphics/Private/AnimatedMeshRenderer.cpp Graphics/Public/AnimatedMeshRenderer.hpp Graphics/Public/Animation.hpp Graphics/Private/AnimatedModel.cpp Graphics/Public/AnimatedModel.hpp Graphics/Private/Animator.cpp Graphics/Public/Animator.hpp Graphics/Private/Animation.cpp Graphics/Private/AnimationSystem.cpp Graphics/Public/AnimationSystem.hpp)

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