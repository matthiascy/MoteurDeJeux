#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_CORE_CONTAINER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_CORE_CONTAINER_HPP

#include "ContainerTypes.hpp"

template <typename Key, typename Value>
inline bool contains(const HashMap<Key, Value>& hashMap, const Key& key)
{
  return hashMap.find(key) != hashMap.end();
}

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_CORE_CONTAINER_HPP */
