#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "Thirdparty/fmod/fmod.hpp"
#include "Thirdparty/fmod/fmod_studio.hpp"
#include "map"
#include "string"
#include "../System.hpp"

class AudioSystem : public System
{
public:
    AudioSystem(const String& name, Engine* engine, Object* parent = nullptr);
    ~AudioSystem();
    void chargerSon(std::string nomAMapper, std::string nomFichier);
    void jouerSon(std::string nomMappe);
    void dechargerSon(std::string nomMappe){FMOD_Sound_Release(m_sons[nomMappe]);}
    void setParam(std::string nomEvent, std::string nomParam, float valParam){FMOD_Studio_EventInstance_SetParameterByName(m_mapEvents[nomEvent],nomParam.c_str(), valParam, false);}
    float getParam(std::string nomEvent, std::string nomParam);
    void chargerBanque(std::string nomAMapper, std::string nomFichier);
    void dechargerBanque(std::string nomMappe){FMOD_Studio_Bank_Unload(m_banques[nomMappe]);}
    void chargerEvent(std::string nomEvent,std::string pathFmod);//premier paramètre est le nom à mapper, le deuxième et le chemin que on peu copier de FMOD. Note : il faut charger la banque de strings (Master.strings.bank), sous n'importe quel nom.
    void jouerEvent(std::string nomEvent);
    void stopperEvent(std::string nomEvent);
    void dechargerEvent(std::string nomMappe){FMOD_Studio_EventInstance_Release(m_mapEvents[nomMappe]);}

    void init() override;

    void fixedUpdate(Real dt) override { };

    void preUpdate(Real dt) override { };

    void update(Real dt) override { };

    void postUpdate(Real dt) override { };

protected :
    FMOD_SYSTEM* m_system;
    FMOD_STUDIO_SYSTEM* m_studio;
    std::map<std::string, FMOD_STUDIO_EVENTINSTANCE*> m_mapEvents;
    std::map<std::string, FMOD_STUDIO_BANK*> m_banques;
    std::map<std::string, FMOD_SOUND*> m_sons;
};

#endif // AUDIOSYSTEM_H
