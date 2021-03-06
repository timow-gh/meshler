#ifndef MESHLER_RENDERABLEBUILDER_H
#define MESHLER_RENDERABLEBUILDER_H

#include <Core/Types/TVector.hpp>
#include <Graphics/Renderables/RendereableId.hpp>
#include <functional>
#include <math.h>

namespace Graphics
{
class View;
class PointRenderable;
class LineRenderable;
class TriangleRenderable;
} // namespace Graphics

namespace Geometry
{
template <typename TFloatType, typename TIndexType>
struct HalfedgeMesh;
}

namespace Meshler
{
class MSphere;
class MCone;
class MCylinder;
class MSegments;
class MCuboid;
class MGrid;
struct PresenterConfig;

class RenderableBuilder {
    std::reference_wrapper<Graphics::View> m_view;
    Core::TVector<Graphics::RenderableId> m_renderableIds;

    Core::TVector<std::reference_wrapper<const MSphere>> m_spheres;
    Core::TVector<std::reference_wrapper<const MCone>> m_cones;
    Core::TVector<std::reference_wrapper<const MCylinder>> m_cylinders;
    Core::TVector<std::reference_wrapper<const MSegments>> m_segments;
    Core::TVector<std::reference_wrapper<const MCuboid>> m_cuboids;
    Core::TVector<std::reference_wrapper<const MGrid>> m_grids;
    std::reference_wrapper<const PresenterConfig> m_presenterConfig;

  public:
    RenderableBuilder(Graphics::View& view, const PresenterConfig& presenterConfig)
        : m_view(view), m_presenterConfig(presenterConfig)
    {
    }

    void add(const MSphere& sphere);
    void add(const MCone& cone);
    void add(const MCylinder& cylinder);
    void add(const MSegments& segments);
    void add(const MCuboid& cuboid);
    void add(const MGrid& grid);

    void setPresenterConfig(const PresenterConfig& presenterConfig);

    Core::TVector<Graphics::RenderableId> build();

  private:
    template <typename TRenderable>
    void addRenderable(TRenderable&& renderable);

    void build(const MSphere& sphere);
    void build(const MCone& cone);
    void build(const MCylinder& cylinder);
    void build(const MSegments& segments);
    void build(const MCuboid& cuboid);
    void build(const MGrid& grid);

    CORE_NODISCARD Core::TVector<Graphics::Vertex>
    buildSegmentVertices(const Core::TVector<Geometry::Segment3d>& segments) const;

    CORE_NODISCARD Graphics::LineRenderable
    createLineRenderables(const Core::TVector<Geometry::Segment3d>& segments) const;

    CORE_NODISCARD Graphics::TriangleRenderable createTriangleRenderable(
        const Geometry::HalfedgeMesh<double_t, std::size_t>& halfedgeMesh) const;

    CORE_NODISCARD Core::TVector<Graphics::Vertex>
    segmentGraphicsVertices(const Geometry::HalfedgeMesh<double_t, std::size_t>& heMesh) const;
};
} // namespace Meshler

#endif // MESHLER_RENDERABLEBUILDER_H
