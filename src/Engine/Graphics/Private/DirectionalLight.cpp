#include <Graphics/Public/DirectionalLight.hpp>

DirectionalLight::DirectionalLight(const String& name, GameObject* gameObject,
                                   const Vec3& color, Real intensity)
  : Light(name, gameObject, color, intensity, ELightType::Directional)
{ }

UInt64 DirectionalLight::typeID() const {
  return Component::family::type<DirectionalLight>;
}
