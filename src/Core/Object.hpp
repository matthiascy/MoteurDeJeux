#ifndef MOTEUR_DE_JEUX_SRC_CORE_OBJECT_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_OBJECT_HPP

#include "BasicTypes.hpp"
#include "ContainerTypes.hpp"

class Object {
private:
  String m_name;
  UInt64 m_uuid;

public:
  StringView name() const;
  void setName(const String& name);

  UInt64 uid() const;

  inline explicit operator bool() const;
  inline bool operator==(const Object& other) const;
  inline bool operator!=(const Object& other) const;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_OBJECT_HPP */
