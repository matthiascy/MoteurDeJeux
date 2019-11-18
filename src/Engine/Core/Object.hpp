#ifndef MOTEUR_DE_JEUX_SRC_CORE_OBJECT_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_OBJECT_HPP

#include "Container.hpp"

class Object {
protected:
  String m_name;
  String m_uuid;

public:
  Object() = delete;
  explicit Object(String name);
  virtual ~Object() = default;

  virtual StringView name() const;
  virtual void setName(const String& name);

  virtual StringView uuid() const;

  virtual inline bool operator==(const Object& other) const;
  virtual inline bool operator!=(const Object& other) const;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_OBJECT_HPP */
