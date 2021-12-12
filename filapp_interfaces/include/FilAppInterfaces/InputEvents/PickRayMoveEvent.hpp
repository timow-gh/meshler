#ifndef FILAPP_PICKRAYMOVEEVENT_HPP
#define FILAPP_PICKRAYMOVEEVENT_HPP

#include <FilAppInterfaces/InputEvents/PickRayEvent.hpp>

namespace FilApp
{
struct PickRayMoveEvent : public PickRayEvent
{
    PickRayMoveEvent(const Vec3& origin, const Vec3& direction, double_t time)
        : PickRayEvent(origin, direction, time)
    {
    }
};
} // namespace FilApp

#endif // FILAPP_PICKRAYMOVEEVENT_HPP