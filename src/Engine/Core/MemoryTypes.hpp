#ifndef MOTEUR_DE_JEUX_SRC_CORE_MEMORY_TYPES_HPP
#define MOTEUR_DE_JEUX_SRC_CORE_MEMORY_TYPES_HPP

/* =================== Memory =================== */
#include <QSharedPointer>

template <typename T>
using SharedPtr = QSharedPointer<T>;

template <typename T>
using UniquePtr = std::unique_ptr<T>;

#endif  /* !MOTEUR_DE_JEUX_SRC_CORE_MEMORY_TYPES_HPP */
