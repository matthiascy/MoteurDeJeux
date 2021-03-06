#include <Core/Public/Object.hpp>
#include <Core/Public/Core.hpp>

Object::Object(const String& name, Object* parent)
  : QObject(parent),
    m_uuid{Uuid::createUuid().toString()}
{
  setObjectName(name);
}

StringView Object::name() const
{
  return objectName();
}

void Object::setName(String&& name)
{
  setObjectName(name);
}

inline StringView Object::uuid() const
{
  return m_uuid;
}

bool Object::compareUUID(const Object& other) const {
  return m_uuid == other.m_uuid;
}