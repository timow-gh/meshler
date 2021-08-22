
#ifndef FILAPP_VIEWLISTENER_H
#define FILAPP_VIEWLISTENER_H

#include "ViewEvents.h"

namespace FilApp
{
class ViewListener
{
  public:
    ~ViewListener() = default;

    virtual void mouseDownEvent(const MouseDownEvent& mouseDownEvent) = 0;
    virtual void mouseUpEvent(const MouseUpEvent& mouseUpEvent) = 0;
    virtual void mouseMovedEvent(const MouseMovedEvent& mouseMovedEvent) = 0;
    virtual void mouseScrollEvent(const MouseScrollEvent& mouseScrollEvent) = 0;
    virtual void keyDownEvent(const KeyDownEvent& keyDownEvent) = 0;
    virtual void keyUpEvent(const KeyUpEvent& keyUpEvent) = 0;

};
} // namespace FilApp
#endif // FILAPP_VIEWLISTENER_H