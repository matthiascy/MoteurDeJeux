#include <GameFramework/Public/ECS/System.hpp>

System::System(const String& name, Engine* engine, Object* parent)
  : Object(name, parent), m_engine{engine}
{ }

