#ifndef MOTEUR_DE_JEUX_SRC_CORE_OBJECT_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_OBJECT_HPP

#include "Container.hpp"
#include <QObject>

class Object : public QObject {
  Q_OBJECT

protected:
  String m_uuid;

public:
  Object() = delete;
  explicit Object(const String& name, Object* parent = nullptr);
  ~Object() override = default;

  [[nodiscard]]
  StringView name() const;
  void setName(String&& name);

  [[nodiscard]]
  StringView uuid() const;

  [[nodiscard]]
  inline bool compareUUID(const Object& other) const;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_OBJECT_HPP */
