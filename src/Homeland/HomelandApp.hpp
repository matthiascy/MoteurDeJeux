#ifndef MOTEUR_DE_JEUX_SRC_HOMELAND_HOMELAND_APP_HPP
#define MOTEUR_DE_JEUX_SRC_HOMELAND_HOMELAND_APP_HPP

#include <GameFramework/GameApp.hpp>

class HomelandApp : public GameApp {
public:
  HomelandApp(int argc, char** argv);
  void run() override;
};


#endif  /* !MOTEUR_DE_JEUX_SRC_HOMELAND_HOMELAND_APP_HPP */
