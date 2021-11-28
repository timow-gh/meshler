#ifndef FILAPP_IAPPLICATION_HPP
#define FILAPP_IAPPLICATION_HPP

#include "IWindow.hpp"

namespace FilApp
{
class IApplication
{
  public:
    virtual ~IApplication() = default;

    virtual IWindow* getWindow() = 0;
};
} // namespace FilApp

#endif // FILAPP_IAPPLICATION_HPP