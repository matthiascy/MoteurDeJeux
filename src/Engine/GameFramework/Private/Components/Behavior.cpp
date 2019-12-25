#include "GameFramework/Public/Components/Behavior.hpp"

Behavior::Behavior(const String& name, GameObject* gameObject)
  : Component(name, gameObject)
{ }

void Behavior::setUpdateFn(void(*fn)(GameObject*, Engine*, Real))
{
  m_update_fn = fn;
}

void Behavior::setFixedUpdateFn(void(*fn)(GameObject*, Engine*, Real))
{
  m_fixed_update_fn = fn;
}

void Behavior::invokeUpdate(GameObject* self, Engine* engine, Real dt)
{
  if (m_update_fn)
    m_update_fn(self, engine, dt);
}

void Behavior::invokeFixedUpdate(GameObject* self, Engine* engine, Real dt)
{
  if (m_fixed_update_fn)
    m_fixed_update_fn(self, engine, dt);
}
