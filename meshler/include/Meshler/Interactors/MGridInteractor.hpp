#ifndef MESHLER_MGRIDINTERACTOR_HPP
#define MESHLER_MGRIDINTERACTOR_HPP

#include <Meshler/MModel.hpp>
#include <Meshler/GeometryElements/MGrid.hpp>
#include <Geometry/Plane.hpp>
#include <Graphics/InputEvents/RayPickEventListener.hpp>

namespace FlowMesh
{

class MGridInteractor : public Graphics::RayPickEventListener {
    MModel* m_model{nullptr};
    double_t m_updateDistance{6.0};
    double_t m_diffUpdateDistance{2.0};
    Geometry::Plane<double_t> m_plane;
    MGrid m_minimalGrid{};
    LinAl::Vec3d m_pevGridIntersectionPoint{0, 0, 0};
    FGuid m_activeGridGuid;

  public:
    static std::unique_ptr<MGridInteractor> create(MModel& model,
                                                          const MGrid& grid);

  private:
    MGridInteractor(MModel& model,
                           Geometry::Plane<double_t> plane,
                           const FGuid& fGuid);

    [[nodiscard]] std::optional<LinAl::Vec3d>
    rayFromEvent(const Graphics::PickRayEvent& pickRayEvent) const;

    void onEvent(const Graphics::PickRayEvent& pickRayEvent) override;
    void onEvent(const Graphics::PickRayMoveEvent& pickRayMoveEvent) override;
};

} // namespace FlowMesh

#endif // MESHLER_MGRIDINTERACTOR_HPP