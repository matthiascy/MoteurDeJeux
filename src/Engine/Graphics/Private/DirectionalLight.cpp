#include <Graphics/Public/DirectionalLight.hpp>

DirectionalLight::DirectionalLight(String name, GameObject* gameObject,
                                   const Vec3& color, Real intensity)
  : Light(std::move(name), gameObject, color, intensity)
{ }
