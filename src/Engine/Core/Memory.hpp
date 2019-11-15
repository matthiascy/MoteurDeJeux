#ifndef MOTEUR_DE_JEUX_SRC_CORE_MEMORY_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_MEMORY_HPP

#include "MemoryTypes.hpp"

namespace memory {
  template <typename T, typename... Args>
  inline auto mkUnique(Args&&... args) -> decltype(std::make_unique<T>(std::forward<Args>(args)...)) {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  inline auto mkShared(Args&&... args) -> decltype(std::make_shared<T>(std::forward<Args>(args)...)) {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }
}

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_MEMORY_HPP */
