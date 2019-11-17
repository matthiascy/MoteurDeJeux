#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_GAME_OBJECT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_GAME_OBJECT_HPP

#include <Engine/Core/CoreTypes.hpp>
#include <Engine/GameFramework/GameFrameworkTypes.hpp>
#include "Types.hpp"
#include <Engine/Graphics/Mesh.hpp>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

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
  Array<ComponentHandle> m_components;

public:
  GameObject() = delete;
  GameObject(String name, Scene* scene);
  GameObject(String name, String tag, EComponentType components = EComponentType::None);
  virtual ~GameObject() = default;

  StringView tag() const;
  void setTag(const String& tag);

  void setTransform(Transform* trans);
  const Transform* transform() const;
  Transform* transform();

  bool isVisible() const;
  void setVisible(bool isVisible);

  bool isStatic() const;
  void setStatic(bool isStatic);

  Scene* scene() const;

  template <typename T>
  ComponentHandle getComponent() const;

  template <typename T>
  ComponentHandle getComponentInParent() const;

  template <typename T>
  ComponentHandle getComponentInChildren() const;

  Array<ComponentHandle> getAllComponents() const;

  template <typename T>
  Array<ComponentHandle> getComponents() const;

  template <typename T>
  Array<ComponentHandle> getComponentsInParent() const;

  template <typename T>
  Array<ComponentHandle> getComponentsInChildren() const;

  template <typename T>
  bool hasComponent() const;

  bool hasComponent(EComponentType componentType) const;

  template <typename T, class... Args>
  void addComponent(Args&&... args);

  void addComponent(ComponentHandle component);
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_GAME_OBJECT_HPP */
