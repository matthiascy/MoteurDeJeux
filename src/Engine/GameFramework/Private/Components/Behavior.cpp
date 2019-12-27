#include "GameFramework/Public/Components/Behavior.hpp"

Behavior::Behavior(const String& name, GameObject* gameObject)
  : Component(name, gameObject), m_update_fns{}, m_fixed_update_fns{}
{ }

void Behavior::addUpdateFunction(void(*fn)(GameObject*, Engine*, Real))
{
  if (fn) {
    m_update_fns.push_back(fn);
  }
}

void Behavior::addFixedUpdateFunction(void(*fn)(GameObject*, Engine*, Real))
{
  if (fn) {
    m_fixed_update_fns.push_back(fn);
  }
}

void Behavior::invokeUpdates(GameObject* self, Engine* engine, Real dt)
{
  for (auto& fn : m_update_fns)
    fn(self, engine, dt);
}

void Behavior::invokeFixedUpdates(GameObject* self, Engine* engine, Real dt)
{
  for (auto& fn : m_fixed_update_fns)
    fn(self, engine, dt);
}
