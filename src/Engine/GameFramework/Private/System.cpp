#include <GameFramework/System.hpp>

System::System(String name, Engine* engine)
  : Object(std::move(name)), m_engine{engine}
{ }

