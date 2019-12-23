#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_CORE_CONTAINER_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_CORE_CONTAINER_HPP

/* =================== Containers types =================== */
#include <QtGlobal>
#include <QVector>
#include <QString>
#include <QHash>
#include <QPair>

template <typename Key, typename Value>
using HashMap = QHash<Key, Value>;

template <typename T>
using Array = QVector<T>;

template <typename T, typename U>
using Pair = QPair<T, U>;

typedef QString     String;

/* QStringView is introduced in Qt5.10.0 */
#if QT_VERSION >= 0x0501000
#include <QStringView>
typedef QStringView StringView;
#else
typedef QString StringView;
#endif

template <typename Key, typename Value>
inline bool contains(const HashMap<Key, Value>& hashMap, const Key& key)
{
  return hashMap.find(key) != hashMap.end();
}

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_CORE_CONTAINER_HPP */
