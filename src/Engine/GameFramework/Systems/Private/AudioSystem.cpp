#include "GameFramework/Systems/AudioSystem.hpp"

AudioSystem::AudioSystem(const String &name, Engine *engine, Object *parent) : System(name, engine, parent)
{
    FMOD_System_Create(&m_system);
    FMOD_System_Init(m_system, 32, FMOD_INIT_NORMAL, NULL);

    FMOD::Studio::System::create(&m_studio,FMOD_VERSION);
    m_studio->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL);

}

AudioSystem::~AudioSystem(){

    FMOD_System_Close(m_system);
    FMOD_System_Release(m_system);

    m_studio->release();
}

void AudioSystem::jouerSon(std::string nomFichier){

    FMOD_SOUND *son = NULL;
    FMOD_System_CreateSound(m_system, nomFichier.c_str(), FMOD_CREATESAMPLE, 0, &son);
    FMOD_System_PlaySound(m_system, son, NULL, 0, NULL);
    FMOD_Sound_Release(son);

}

float AudioSystem::getParam(std::string nomEvent, std::string nomParam){
    float ret;
    m_mapEvents[nomEvent]->getParameterByName(nomParam.c_str(), &ret);
    return ret;
}

void AudioSystem::chargerBanque(std::string nomAMapper, std::string nomFichier){

    FMOD::Studio::Bank* banque = NULL;
    m_studio->loadBankFile(nomFichier.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &banque) ;
    m_banques[nomAMapper] = banque;

}

void AudioSystem::chargerEvent(std::string nomEvent){

    FMOD::Studio::EventDescription* desc = NULL;
    m_studio->getEvent(nomEvent.c_str(), &desc);
    if(desc != NULL){
        FMOD::Studio::EventInstance* instance = NULL;
        desc->createInstance(&instance);
        if(instance != NULL) m_mapEvents[nomEvent]=instance;
    }

}

void AudioSystem::jouerEvent(std::string nomEvent){

    m_mapEvents[nomEvent]->start();

}

void AudioSystem::stopperEvent(std::string nomEvent){

    m_mapEvents[nomEvent]->stop(FMOD_STUDIO_STOP_IMMEDIATE);

}
