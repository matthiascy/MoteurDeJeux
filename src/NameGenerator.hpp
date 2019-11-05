#ifndef NAMEGENERATOR_HPP
#define NAMEGENERATOR_HPP

#include "Types.hpp"


class NameGenerator {
public:
  explicit NameGenerator(String baseName);
  String generate();

protected:
  String m_base;
  UInt64 m_next;
};

#endif // NAMEGENERATOR_HPP
