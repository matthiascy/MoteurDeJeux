#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_TYPES_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_TYPES_HPP

#include <Core/Core.hpp>

using GameObjectID = String;
using ComponentTypeID = UInt64 ;

template<typename...>
class Family {
  inline static uint64_t identifier{};

public:
  /** @brief Unsigned integer type. */
  using family_type = uint64_t;

  /** @brief Statically generated unique identifier for the given type. */
  template<typename... Type>
  inline static const family_type type = identifier++;
};

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_TYPES_HPP */
