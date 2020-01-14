#include <GameFramework/Public/Systems/AudioSystem.hpp>
#include <Core/Public/Container.hpp>
#include <GameFramework/Public/Engine.hpp>
#include <GameFramework/Public/ECS/System.hpp>

AudioSystem::AudioSystem(const String &name, Engine *engine, Object *parent) : System(name, engine, parent)
{
    FMOD_System_Create(&m_system);
    FMOD_System_Init(m_system, 32, FMOD_INIT_NORMAL, NULL);

    FMOD_Studio_System_Create(&m_studio,FMOD_VERSION);
    FMOD_Studio_System_Initialize(m_studio,32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL);

}

AudioSystem::~AudioSystem(){

    for(auto const& son : m_sons){
        dechargerSon(son.first);
    }

    for(auto const& event : m_mapEvents){
        dechargerEvent(event.first);
    }

    for(auto const& banque : m_banques){
        dechargerBanque(banque.first);
    }

    FMOD_System_Close(m_system);
    FMOD_System_Release(m_system);

    FMOD_Studio_System_Release(m_studio);
}

void AudioSystem::chargerSon(std::string nomAMapper, std::string nomFichier){

    FMOD_SOUND *son = NULL;
    FMOD_System_CreateSound(m_system, nomFichier.c_str(), FMOD_CREATESAMPLE, 0, &son);
    if(son != NULL) m_sons[nomAMapper] = son;
}

void AudioSystem::jouerSon(std::string nomMappe){
    FMOD_System_PlaySound(m_system, m_sons[nomMappe], NULL, 0, NULL);
}

float AudioSystem::getParam(std::string nomEvent, std::string nomParam){
    float ret;
    FMOD_Studio_EventInstance_GetParameterByName(m_mapEvents[nomEvent], nomParam.c_str(), &ret, NULL);
    return ret;
}

void AudioSystem::chargerBanque(std::string nomAMapper, std::string nomFichier){

    FMOD_STUDIO_BANK* banque = NULL;
    FMOD_Studio_System_LoadBankFile(m_studio, nomFichier.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &banque);
    m_banques[nomAMapper] = banque;

}

void AudioSystem::chargerEvent(std::string nomEventAMapper, std::string pathFmod){

    FMOD_STUDIO_EVENTDESCRIPTION* desc = NULL;
    FMOD_Studio_System_GetEvent(m_studio, pathFmod.c_str(), &desc);
    if(desc != NULL){
        FMOD_STUDIO_EVENTINSTANCE* instance = NULL;
        FMOD_Studio_EventDescription_CreateInstance(desc, &instance);
        if(instance != NULL) m_mapEvents[nomEventAMapper]=instance;
    }

}

void AudioSystem::jouerEvent(std::string nomEvent){

    FMOD_Studio_System_FlushCommands(m_studio);
    FMOD_Studio_EventInstance_Start(m_mapEvents[nomEvent]);
    FMOD_Studio_System_FlushCommands(m_studio);

}
void AudioSystem::stopperEvent(std::string nomEvent){

    FMOD_Studio_System_FlushCommands(m_studio);
    FMOD_Studio_EventInstance_Stop(m_mapEvents[nomEvent], FMOD_STUDIO_STOP_IMMEDIATE);
    FMOD_Studio_System_FlushCommands(m_studio);

}
