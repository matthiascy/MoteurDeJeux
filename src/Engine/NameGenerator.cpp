#include "NameGenerator.hpp"

NameGenerator::NameGenerator(String baseName)
  : m_base{baseName}, m_next{1}
{ }

String NameGenerator::generate()
{
  return m_base + String::number(m_next++);
}
