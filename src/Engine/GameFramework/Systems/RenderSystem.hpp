#ifndef MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP
#define MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP

#include <GameFramework/System.hpp>
#include <QOpenGLContext>
#include <QOffscreenSurface>

// TODO forward
class GameObject;
class Engine;

class RenderSystem : public System {
private:
  UniquePtr<QOpenGLContext>    m_context;
  UniquePtr<QOffscreenSurface> m_surface;

public:
  RenderSystem(String name, Engine* engine);
  ~RenderSystem() override;

  void init() override;

  void render(GameObject* gameObject);

};

#endif  /* !MOTEUR_DE_JEUX_SRC_ENGINE_GAME_FRAMEWORK_SUBSYSTEMS_RENDERER_SYSTEM_HPP */
