#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BEHAVIOR_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BEHAVIOR_HPP

#include <Core/Public/Core.hpp>
#include <GameFramework/Public/ECS/Component.hpp>

class Engine;

class Behavior : public Component {
public:

  using BehaviorFunction = std::function<void(GameObject*, Engine*, Real)>;

  Behavior(const String& name, GameObject* gameObject);
  ~Behavior() override = default;

  [[nodiscard]]
  UInt64 typeID() const override {
    return Component::family::type<Behavior>;
  }

  template <typename T>
  void setUpdateFn(T* object, void(T::*fn)(GameObject*, Engine*, Real));

  void setUpdateFn(void(*fn)(GameObject*, Engine*, Real));

  template <typename T>
  void setFixedUpdateFn(T* object, void(T::*fn)(GameObject*, Engine*, Real));

  void setFixedUpdateFn(void(*fn)(GameObject*, Engine*, Real));

  void invokeFixedUpdate(GameObject* self, Engine* engine, Real dt);

  void invokeUpdate(GameObject* self, Engine* engine, Real dt);

private:
  BehaviorFunction m_update_fn;
  BehaviorFunction m_fixed_update_fn;
};

template<typename T>
void Behavior::setUpdateFn(T *object, void (T::*fn)(GameObject *, Engine *, Real))
{
  using namespace std::placeholders;
  m_update_fn = std::bind(fn, object, _1, _2, _3);
}

template <typename T>
void Behavior::setFixedUpdateFn(T* object, void(T::*fn)(GameObject*, Engine*, Real))
{
  using namespace std::placeholders;
  m_fixed_update_fn = std::bind(fn, object, _1, _2, _3);
}


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BEHAVIOR_HPP */
