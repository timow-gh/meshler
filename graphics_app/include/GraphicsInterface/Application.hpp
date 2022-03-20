#ifndef FILAPP_APPLICATION_HPP
#define FILAPP_APPLICATION_HPP

#include <GraphicsInterface/Window.hpp>

namespace Graphics
{
class Application {
  public:
    virtual ~Application() = default;

    virtual Window* getWindow() = 0;
};
} // namespace Graphics

#endif // FILAPP_APPLICATION_HPP