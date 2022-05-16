#ifndef FILAPP_FLOWMESHGRIDINTERACTOR_HPP
#define FILAPP_FLOWMESHGRIDINTERACTOR_HPP

#include <FlowMesh/FlowMeshModel.hpp>
#include <FlowMesh/GeometryElements/FlowMeshGrid.hpp>
#include <Geometry/Plane.hpp>
#include <GraphicsInterface/InputEvents/RayPickEventListener.hpp>

namespace FlowMesh
{

class FlowMeshGridInteractor : public Graphics::RayPickEventListener {
    FlowMeshModel* m_model{nullptr};
    double_t m_updateDistance{6.0};
    double_t m_diffUpdateDistance{2.0};
    Geometry::Plane<double_t> m_plane;
    FlowMeshGrid m_minimalGrid{};
    LinAl::Vec3d m_pevGridIntersectionPoint{0, 0, 0};
    FGuid m_activeGridGuid;

  public:
    static std::unique_ptr<FlowMeshGridInteractor> create(FlowMeshModel& model,
                                                          const FlowMeshGrid& grid);

  private:
    FlowMeshGridInteractor(FlowMeshModel& model,
                           Geometry::Plane<double_t> plane,
                           const FGuid& fGuid);

    [[nodiscard]] std::optional<LinAl::Vec3d>
    rayFromEvent(const Graphics::PickRayEvent& pickRayEvent) const;

    void onEvent(const Graphics::PickRayEvent& pickRayEvent) override;
    void onEvent(const Graphics::PickRayMoveEvent& pickRayMoveEvent) override;
};

} // namespace FlowMesh

#endif // FILAPP_FLOWMESHGRIDINTERACTOR_HPP
