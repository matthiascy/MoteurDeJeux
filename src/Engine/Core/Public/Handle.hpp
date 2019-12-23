#ifndef MOTEUR_DE_JEUX_SRC_CORE_HANDLE_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_HANDLE_HPP

#include "BasicTypes.hpp"
#include "Container.hpp"

enum class EHandleType : UInt32 {
  GameObject,
  TransformComponent,
  IndexBuffer,
  Shader,
  Texture,
  VertexBuffer,
  None,
  Count
};

struct Handle {
  Handle()
      : m_idx{0}, m_type{EHandleType::None}
  { }

  Handle(UInt32 idx, EHandleType type)
      : m_idx{idx}, m_type{type}
  { }

  UInt64 idx() const { return m_idx; }

  bool operator==(const Handle& other) const {
    return (m_idx == other.m_idx) && (m_type == other.m_type);
  }

  bool operator!=(const Handle& other) const {
    return (m_idx != other.m_idx) || (m_type == other.m_type);
  }

  bool operator<(const Handle& other) const {
    return (m_type == other.m_type) && (m_idx < other.m_idx);
  }

private:
  UInt32      m_idx  = 0;
  EHandleType m_type = EHandleType::None;
};

String typeNameOf(Handle handle);

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_HANDLE_HPP */
