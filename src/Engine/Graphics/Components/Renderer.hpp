#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP
#define MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP

#include <GameFramework/Component.hpp>

template <typename T>
class Renderer : public Component<T> {
private:
public:
  Renderer(const String& name, GameObject* gameObject);
  ~Renderer() override = default;
};

#include "Engine/Graphics/Components/Private/Renderer.inl"

#endif  /* !MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP */
