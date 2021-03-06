#include <Core/Math/Eps.hpp>
#include <Meshler/Interactors/MGridInteractor.hpp>
#include <Meshler/LinAlConversion.hpp>
#include <Meshler/MModel.hpp>
#include <utility>

namespace Meshler
{

MGridInteractor::MGridInteractor(MModel& model,
                                 Geometry::Plane<double_t> plane,
                                 const FGuid& activeGridGuid)
    : m_model(&model), m_plane(std::move(plane)), m_activeGridGuid(activeGridGuid)
{
}

std::optional<LinAl::Vec3d>
MGridInteractor::rayIntersection(const Graphics::PickRayEvent& pickRayEvent) const
{

    const Geometry::Ray3<double_t> ray{vec3ToLinAlVec3<double_t>(pickRayEvent.origin),
                                       vec3ToLinAlVec3<double_t>(pickRayEvent.direction)};

    return m_plane.intersection(ray);
}

void MGridInteractor::onEvent(const Graphics::PickRayEvent& pickRayEvent)
{
    std::optional<LinAl::Vec3d> result = rayIntersection(pickRayEvent);
    if (!result)
        return;
    LinAl::Vec3d& vec = *result;

    if (Core::isGreater(vec[0], m_minimalGrid.getMaxXLength()))
    {
        m_minimalGrid.setMaxXLength(vec[0]);
    }
    if (Core::isGreater(vec[1], m_minimalGrid.getMaxYLength()))
    {
        m_minimalGrid.setMaxYLength(vec[1]);
    }
    if (Core::isLess(vec[0], m_minimalGrid.getMinXLength()))
    {
        m_minimalGrid.setMinXLength(vec[0]);
    }
    if (Core::isLess(vec[1], m_minimalGrid.getMinYLength()))
    {
        m_minimalGrid.setMinYLength(vec[1]);
    }

    m_minimalGrid.updateSegments();
    m_model->remove(m_activeGridGuid);
    MGrid newGrid = m_minimalGrid;
    m_activeGridGuid = newGrid.getFGuid();
    m_model->add(newGrid);
}

void MGridInteractor::onEvent(const Graphics::PickRayMoveEvent& pickRayMoveEvent)
{
    std::optional<LinAl::Vec3d> result = rayIntersection(pickRayMoveEvent);
    if (!result)
        return;

    double_t diff = LinAl::norm2(LinAl::Vec3d{*result - m_pevGridIntersectionPoint});
    if (Core::isLess(diff, m_diffUpdateDistance))
        return;
    m_pevGridIntersectionPoint = *result;

    double_t xCoord = (*result)[0];
    double_t yCoord = (*result)[1];

    auto* activeGrid = m_model->get<MGrid>(m_activeGridGuid);
    if (!activeGrid)
        return;

    double_t flooredXCoord = std::floor(xCoord);
    bool updated{false};
    if (Core::isLess(xCoord, m_minimalGrid.getMinXLength() + m_updateDistance) &&
        Core::isLess(xCoord, activeGrid->getMinXLength() + m_updateDistance))
    {
        // Extend xMin to cursor
        activeGrid->setMinXLength(flooredXCoord - m_updateDistance);
        updated = true;
    }
    else if (Core::isLess(xCoord, m_minimalGrid.getMinXLength() + m_updateDistance) &&
             Core::isGreater(xCoord, activeGrid->getMinXLength() + m_updateDistance))
    {
        // Shrink xMin to cursor
        activeGrid->setMinXLength(flooredXCoord - m_updateDistance);
        updated = true;
    }
    else if (Core::isGreater(xCoord, m_minimalGrid.getMinXLength() + m_updateDistance) &&
             !Core::isEq(activeGrid->getMinXLength(),
                         m_minimalGrid.getMinXLength() + m_updateDistance))
    {
        // Shrink xMin to minimal grid
        activeGrid->setMinXLength(m_minimalGrid.getMinXLength());
        updated = true;
    }

    double_t flooredYCoord = std::floor(yCoord);
    if (Core::isLess(yCoord, (m_minimalGrid.getMinYLength() + m_updateDistance)) &&
        Core::isLess(yCoord, (activeGrid->getMinYLength() + m_updateDistance)))
    {
        // Extend yMin to cursor
        activeGrid->setMinYLength(flooredYCoord - m_updateDistance);
        updated = true;
    }
    else if (Core::isLess(yCoord, (m_minimalGrid.getMinYLength() + m_updateDistance)) &&
             Core::isGreater(yCoord, activeGrid->getMinYLength() + m_updateDistance))
    {
        // Shrink yMin to cursor
        activeGrid->setMinYLength(flooredYCoord - m_updateDistance);
        updated = true;
    }
    else if (Core::isGreater(yCoord, m_minimalGrid.getMinYLength() + m_updateDistance) &&
             !Core::isEq(activeGrid->getMinYLength(),
                         m_minimalGrid.getMinYLength() + m_updateDistance))
    {
        // Shrink yMin to minimal grid
        activeGrid->setMinYLength(m_minimalGrid.getMinYLength());
        updated = true;
    }

    double_t ceilXCoord = std::ceil(xCoord);
    if (Core::isGreater(xCoord, m_minimalGrid.getMaxXLength() - m_updateDistance) &&
        Core::isGreater(xCoord, activeGrid->getMaxXLength() - m_updateDistance))
    {
        // Extend xMax to cursor
        activeGrid->setMaxXLength(ceilXCoord + m_updateDistance);
        updated = true;
    }
    else if (Core::isGreater(xCoord, m_minimalGrid.getMaxXLength() - m_updateDistance) &&
             Core::isLess(xCoord, activeGrid->getMaxXLength() - m_updateDistance))
    {
        // Shrink xMax to cursor
        activeGrid->setMaxXLength(ceilXCoord + m_updateDistance);
        updated = true;
    }
    else if (Core::isLess(xCoord, m_minimalGrid.getMaxXLength() - m_updateDistance) &&
             !Core::isEq(activeGrid->getMaxXLength(),
                         m_minimalGrid.getMaxXLength() - m_updateDistance))
    {
        // Shrink xMax to minimal grid
        activeGrid->setMaxXLength(m_minimalGrid.getMaxXLength());
        updated = true;
    }

    double_t ceilYCoord = std::ceil(yCoord);
    if (Core::isGreater(yCoord, m_minimalGrid.getMaxYLength() - m_updateDistance) &&
        Core::isGreater(yCoord, activeGrid->getMaxYLength() - m_updateDistance))
    {
        // Extend yMax to cursor
        activeGrid->setMaxYLength(ceilYCoord + m_updateDistance);
        updated = true;
    }
    else if (Core::isGreater(yCoord, m_minimalGrid.getMaxYLength() - m_updateDistance) &&
             Core::isLess(yCoord, activeGrid->getMaxYLength() - m_updateDistance))
    {
        // Shrink yMax to cursor
        activeGrid->setMaxYLength(ceilYCoord + m_updateDistance);
        updated = true;
    }
    else if (Core::isLess(yCoord, m_minimalGrid.getMaxYLength() - m_updateDistance) &&
             !Core::isEq(activeGrid->getMaxYLength(),
                         m_minimalGrid.getMaxYLength() - m_updateDistance))
    {
        // Shrink yMax to minimal grid
        activeGrid->setMaxYLength(m_minimalGrid.getMaxYLength());
        updated = true;
    }

    if (updated)
    {
        activeGrid->updateSegments();
        m_model->update(m_activeGridGuid);
    }
}

} // namespace Meshler
