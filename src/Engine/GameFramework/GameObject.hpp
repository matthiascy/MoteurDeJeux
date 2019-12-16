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
// TODO: handling multiple collider

class GameObject : public Object {

  using ComponentHandle = Handle;

private:
  String m_tag;
  Transform* m_transform;
  bool m_is_static;   // do we need to update GameObject ?
  bool m_is_visible;  // is the GameObject active in the scene ?
  bool m_is_simulated;
  bool m_has_collider;
  Scene* m_scene;
  AssetHandle m_mesh;
  btRigidBody* m_rigid_body;
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

  void setRigidBody(btRigidBody* rigidBody);

  [[nodiscard]]
  btRigidBody* rigidBody() const;

  [[nodiscard]]
  bool isVisible() const;
  void setVisible(bool isVisible);

  [[nodiscard]]
  bool isStatic() const;
  void setStatic(bool isStatic);

  [[nodiscard]]
  bool hasCollider() const;
  void setHasCollider(bool hasCollider);

  [[nodiscard]]
  bool isSimulated() const;

  void setIsSimulated(bool isSimulated);

  [[nodiscard]] AssetHandle meshHandle() const;

  [[nodiscard]] Scene* scene() const;

  template <typename T,
      typename = std::enable_if_t<std::is_base_of_v<AbstractComponent, T>>>
  T* getComponent() const;

  template <template<typename> class T, typename U,
      typename = std::enable_if_t<std::is_base_of_v<AbstractComponent, T>>>
  T<U>* getComponent() const;

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

template <typename T, typename>
T* GameObject::getComponent() const
{
  for (auto* c : m_components) {
    if (c->typeID() == T::componentTypeID()) {
      return dynamic_cast<T*>(c);
    }
  }

  return nullptr;
}

template <template<typename> class T, typename U, typename>
T<U>* GameObject::getComponent() const
{
  for (auto* c : m_components) {
    if (c->typeID() == T<U>::componentTypeID()) {
      return dynamic_cast<T<U>*>(c);
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
