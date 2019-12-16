#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP

#include <GameFramework/Component.hpp>
#include <Graphics/MaterialData.hpp>

class Material : Component {
private:
  MaterialData m_data;

public:
  Material(const String& name, GameObject* gameObject);
};


#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_MATERIAL_COMPONENT_HPP */
