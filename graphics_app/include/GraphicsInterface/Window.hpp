#ifndef FILAPP_WINDOW_HPP
#define FILAPP_WINDOW_HPP

#include <vector>

namespace Graphics
{
class View;

class Window {
  public:
    virtual ~Window() = default;

    using WindowId = uint32_t;

    virtual View* getMainIView() = 0;
    virtual std::vector<View*> getIViews() = 0;

    virtual WindowId getIWindowId() = 0;
};
} // namespace Graphics

#endif // FILAPP_WINDOW_HPP