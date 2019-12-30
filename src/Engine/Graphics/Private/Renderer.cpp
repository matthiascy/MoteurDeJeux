#include <Graphics/Public/Renderer.hpp>

Renderer::Renderer(const String& name, GameObject *gameObject)
  : Component(name, gameObject)
{ }

void Renderer::enable_vertex_attrib_array(UInt32 num, OglFnsCore4_0* fns)
{
  for (unsigned i = 0; i < num; ++i) {
    fns->glEnableVertexAttribArray(i);
  }
}
