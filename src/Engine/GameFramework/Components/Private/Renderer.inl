#ifndef MOTEUR_DE_JEUX_SRC_GAME_FRAMEWORK_COMPONENTS_RENDERER_HPP
  #error "Renderer.inl must be included from Renderer.hpp"
#endif

template <typename T>
Renderer<T>::Renderer(const String& name, GameObject *gameObject)
  : Component<T>(name, gameObject)
{ }
