#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H

#include "../../Thirdparty/fmod/fmod.hpp"
#include "../../Thirdparty/fmod/fmod_studio.hpp"
#include "QVector3D"
#include "map"
#include "string"
#include "../System.hpp"

class AudioSystem : public System
{
public:
    AudioSystem(const String& name, Engine* engine, Object* parent = nullptr);
    ~AudioSystem();
    void jouerSon(std::string nomFichier);
    void setParam(std::string nomEvent, std::string nomParam, float valParam){m_mapEvents[nomEvent]->setParameterByName(nomParam.c_str(), valParam);}
    float getParam(std::string nomEvent, std::string nomParam);
    void chargerBanque(std::string nomAMapper, std::string nomFichier);
    void chargerEvent(std::string nomEvent);
    void jouerEvent(std::string nomEvent);
    void stopperEvent(std::string nomEvent);

protected :
    FMOD_SYSTEM* m_system;
    FMOD::Studio::System* m_studio;
    std::map<std::string, FMOD::Studio::EventInstance*> m_mapEvents;
    std::map<std::string, FMOD::Studio::Bank*> m_banques;
};

#endif // AUDIOSYSTEM_H
