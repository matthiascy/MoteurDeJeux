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
  void addUpdateFunction(T* object, void(T::*fn)(GameObject*, Engine*, Real));

  void addUpdateFunction(void(*fn)(GameObject*, Engine*, Real));

  template <typename T>
  void addFixedUpdateFunction(T* object, void(T::*fn)(GameObject*, Engine*, Real));

  void addFixedUpdateFunction(void(*fn)(GameObject*, Engine*, Real));

  void invokeFixedUpdates(GameObject* self, Engine* engine, Real dt);

  void invokeUpdates(GameObject* self, Engine* engine, Real dt);

private:
  Array<BehaviorFunction> m_update_fns;
  Array<BehaviorFunction> m_fixed_update_fns;
};

template<typename T>
void Behavior::addUpdateFunction(T *object, void (T::*fn)(GameObject *, Engine *, Real))
{
  using namespace std::placeholders;
  if (fn) {
    m_update_fns.push_back(std::bind(fn, object, _1, _2, _3));
  }
}

template <typename T>
void Behavior::addFixedUpdateFunction(T* object, void(T::*fn)(GameObject*, Engine*, Real))
{
  using namespace std::placeholders;
  if (fn) {
    m_fixed_update_fns.push_back(std::bind(fn, object, _1, _2, _3));
  }
}


#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_COMPONENTS_BEHAVIOR_HPP */
