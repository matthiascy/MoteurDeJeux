#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_GAME_OBJECT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_GAME_OBJECT_HPP

#include <Core/Core.hpp>
#include <GameFramework/Forward.hpp>
#include <Graphics/Mesh.hpp>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <GameFramework/Assets/AssetsTypes.hpp>

class Scene;

// TODO: instantly all objects have a collider, seperate
// TODO: final

class GameObject : public Object {
  using ComponentHandle = Handle;
private:
  String m_tag;
  Transform* m_transform;
  bool m_is_static;   // do we need to update GameObject ?
  bool m_is_visible;  // is the GameObject active in the scene ?
  Scene* m_scene;
  AssetHandle m_mesh;
  Array<ComponentHandle> m_components;

public:
  GameObject() = delete;
  GameObject(String name, Scene* scene);
  GameObject(String name, Scene* scene, String tag);
  ~GameObject() override = default;

  [[nodiscard]] StringView tag() const;
  void setTag(const String& tag);

  void setTransform(Transform* trans);
  [[nodiscard]] const Transform* transform() const;
  Transform* transform();

  [[nodiscard]] bool isVisible() const;
  void setVisible(bool isVisible);

  [[nodiscard]] bool isStatic() const;
  void setStatic(bool isStatic);

  [[nodiscard]] AssetHandle meshHandle() const;

  [[nodiscard]] Scene* scene() const;

  template <typename T>
  T* getComponent() const;

  template <typename T>
  [[nodiscard]] ComponentHandle getComponentInParent() const;

  template <typename T>
  [[nodiscard]] ComponentHandle getComponentInChildren() const;

  [[nodiscard]] Array<Component*> getComponents() const;

  template <typename T>
  bool hasComponent() const;

  template <typename T, class... Args>
  T* addComponent(Args&&... args);

  template <typename T>
  void removeComponent();

  void removeAllComponents();
};

template <typename T>
T* GameObject::getComponent() const
{
  static_assert(std::is_base_of<Component, T>(), "T is not a Component");
}



#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_GAME_OBJECT_HPP */
