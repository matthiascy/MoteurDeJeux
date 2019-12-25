#ifndef HOMELAND_FAMILY_HPP
#define HOMELAND_FAMILY_HPP

#include <Core/Public/Core.hpp>

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

#endif //HOMELAND_FAMILY_HPP
