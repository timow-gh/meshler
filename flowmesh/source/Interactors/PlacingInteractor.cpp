#include <FlowMesh/Interactors/PlacingInteractor.hpp>

namespace FlowMesh
{

PlacingInteractor::PlacingInteractor(Model* model)
    : m_model(model), m_snapGeometries(m_model->calcModelSnapGeometries())
{
}

PlacingInteractor::~PlacingInteractor() CORE_NOEXCEPT
{
    if (m_sphereGuid)
        m_model->remove(*m_sphereGuid);
}

FlowMeshSphere PlacingInteractor::createSphere(const LinAl::Vec3d& origin)
{
    constexpr double_t radius = 0.2;
    auto sphere = Geometry::Sphere<double_t>{origin, radius};
    return FlowMeshSphere{sphere, newFGuid()};
}

std::optional<LinAl::Vec3d>
PlacingInteractor::calcIntersection(const Graphics::PickRayEvent& pickRayEvent) const
{
    const Graphics::Vec3& pickOrigin = pickRayEvent.origin;
    const Graphics::Vec3& pickDirection = pickRayEvent.direction;
    const Geometry::Ray3<double_t> ray{
        LinAl::Vec3d{pickOrigin[0], pickOrigin[1], pickOrigin[2]},
        LinAl::Vec3d{pickDirection[0], pickDirection[1], pickDirection[2]}};
    return m_snapGeometries.calcSnapPoint(ray);
}

void PlacingInteractor::event(const Graphics::PickRayEvent& pickRayEvent)
{
    if (auto intersection = calcIntersection(pickRayEvent))
    {
        FlowMeshSphere flowMeshSphere = createSphere(*intersection);
        m_model->add(flowMeshSphere);
        m_snapGeometries.add(getSnapPoints(flowMeshSphere.getSphere()));
    }
}

void PlacingInteractor::event(const Graphics::PickRayMoveEvent& pickRayMoveEvent)
{
    if (auto intersection = calcIntersection(pickRayMoveEvent))
    {
        if (!m_sphereGuid)
        {
            FlowMeshSphere flowMeshSphere = createSphere(LinAl::ZERO_VEC3D);
            m_model->add(flowMeshSphere);
            m_sphereGuid = flowMeshSphere.getFGuid();
            return;
        }
        m_model->setPosition(*m_sphereGuid, *intersection);
    }
}
} // namespace FlowMesh
