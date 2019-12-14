#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_GAME_OBJECT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_GAME_OBJECT_HPP

#include <Core/Core.hpp>
#include <GameFramework/Forward.hpp>
#include <Graphics/Mesh.hpp>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <GameFramework/Assets/AssetsTypes.hpp>
#include <GameFramework/Components/Transform.hpp>
#include "AbstractComponent.hpp"

class Scene;

// TODO: instantly all objects have a collider, separate
// TODO: final

class GameObject : public Object {
  Q_OBJECT

  using ComponentHandle = Handle;

private:
  String m_tag;
  Transform* m_transform;
  bool m_is_static;   // do we need to update GameObject ?
  bool m_is_visible;  // is the GameObject active in the scene ?
  Scene* m_scene;
  AssetHandle m_mesh;
  Array<AbstractComponent*> m_components;
  // TODO::using handle to retrieve components

public:
  GameObject() = delete;
  GameObject(const String& name, Scene* scene, Object* parent = nullptr);
  GameObject(const String& name, Scene* scene, const String& tag, Object* parent = nullptr);
  ~GameObject() override = default;

  [[nodiscard]] StringView tag() const;
  void setTag(const String& tag);

  void setTransform(Transform* trans);
  Transform* transform();

  [[nodiscard]]
  const Transform* transform() const;

  [[nodiscard]] bool isVisible() const;
  void setVisible(bool isVisible);

  [[nodiscard]] bool isStatic() const;
  void setStatic(bool isStatic);

  [[nodiscard]] AssetHandle meshHandle() const;

  [[nodiscard]] Scene* scene() const;

  template <typename T>
  T* getComponent() const;

  template <typename T>
  [[nodiscard]]
  T* getComponentInParent() const;

  template <typename T>
  [[nodiscard]]
  T* getComponentInChildren() const;

  [[nodiscard]]
  Array<AbstractComponent*>& getComponents();

  [[nodiscard]]
  const Array<AbstractComponent*>& getComponents() const;

  template <typename T>
  [[nodiscard]] bool hasComponent() const;

  void addComponent(AbstractComponent* comp);
};

template <typename T>
T* GameObject::getComponent() const
{
  static_assert(std::is_base_of<AbstractComponent, T>(), "T is not a Component");

  for (auto* c : m_components) {
    if (c->typeID() == T::componentTypeID()) {
      return dynamic_cast<T*>(c);
    }
  }

  return nullptr;
}

template <typename T>
bool GameObject::hasComponent() const
{
  for (auto* c : m_components) {
    if (c->typeID() == T::componentTypeID()) {
      return true;
    }
  }
  return false;
}

template <typename T>
[[nodiscard]]
T* GameObject::getComponentInParent() const
{
  return m_transform->parent()->gameObject()->getComponent<T>();
}

template <typename T>
[[nodiscard]]
T* GameObject::getComponentInChildren() const
{
  for (auto& c : m_transform->children()) {
    if (c->gameObject()->hasComponent<T>())
      return c->gameObject()->getComponent<T>();
  }
}


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_GAME_OBJECT_HPP */
