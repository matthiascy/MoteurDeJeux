#include "Light.hpp"

Light::Light()
{ }

Light::Light(const String& name)
  : GameObject(name, "Light")
{ }

Light::~Light()
{}
