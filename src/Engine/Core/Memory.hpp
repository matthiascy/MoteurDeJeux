#ifndef MOTEUR_DE_JEUX_SRC_CORE_MEMORY_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_MEMORY_HPP

#include "MemoryTypes.hpp"

template <typename T, typename... Args>
inline auto makeUnique(Args&&... args) -> decltype(std::make_unique<T>(std::forward<Args>(args)...)) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
inline auto makeShared(Args&&... args) -> decltype(std::make_shared<T>(std::forward<Args>(args)...)) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_MEMORY_HPP */
