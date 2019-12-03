#ifndef HOMELAND_HOMELAND_APP_HPP
#define HOMELAND_HOMELAND_APP_HPP

#include <GameFramework/GameApp.hpp>

class Scene;

class HomelandApp : public GameApp {
private:
  Scene* m_main_scene {};

public:
  HomelandApp(int argc, char** argv);
  ~HomelandApp() override;
  void init();
  void run() override;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_HOMELAND_HOMELAND_APP_HPP */
