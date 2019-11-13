#ifndef MOTEUR_DE_JEUX_SRC_CORE_CONTAINER_TYPES_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_CONTAINER_TYPES_HPP

/* =================== Containers types =================== */
#include <QVector>
#include <QString>
#include <QHash>

template <typename Key, typename Value>
using HashMap = QHash<Key, Value>;

template <typename T>
using Array = QVector<T>;

typedef QString     String;
typedef QStringView StringView;

#endif  /* !MOTEURDEJEUX_SRC_CORE_CONTAINERTYPES_HPP */
