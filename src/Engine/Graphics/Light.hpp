#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "GameObject.hpp"

class Light : public GameObject {
public:
  Light();
  explicit Light(const String& name);
  ~Light() override;
};

#endif // LIGHT_HPP
