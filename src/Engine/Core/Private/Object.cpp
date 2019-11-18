#include <Core/Object.hpp>
#include <Core/Core.hpp>
#include <utility>

Object::Object(String name)
  : m_name{std::move(name)},
    m_uuid{Uuid::createUuid().toString()}
{ }

inline StringView Object::name() const
{
  return m_name;
}

void Object::setName(const String& name)
{
  m_name = name;
}

inline StringView Object::uuid() const
{
  return m_uuid;
}

bool Object::operator==(const Object& other) const
{
  return (m_uuid == other.m_uuid);
}

bool Object::operator!=(const Object & other) const
{
  return !(*this == other);
}
