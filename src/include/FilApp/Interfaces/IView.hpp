#ifndef FILAPP_IVIEW_HPP
#define FILAPP_IVIEW_HPP

#include "FilApp/PointRenderable.hpp"
#include "FilApp/TriangleRenderable.hpp"
#include "IInputListener.hpp"
#include "Vec3.hpp"
#include "Viewport.hpp"
#include <filapp_export.h>

namespace FilApp
{
using RenderableIdentifier = uint32_t;

class FILAPP_EXPORT IView
    : public IInputListener
{
  public:
    ~IView() override = default;

    // clang-format off
    virtual auto addRenderable(TriangleRenderable&& renderable) -> RenderableIdentifier = 0;
    virtual auto addRenderable(PointRenderable&& renderable) -> RenderableIdentifier = 0;
    virtual auto getRenderableIdentifiers() const -> std::vector<RenderableIdentifier> = 0;
    virtual void removeRenderable(RenderableIdentifier renderableIdentifier) = 0;
    virtual void clearRenderables() = 0;
    // clang-format on

    virtual void setUsePostprocessing(bool usePostProcessing) = 0;

    virtual void addRotationAnimation(RenderableIdentifier renderableIdentifier,
                                      const Vec3& rotationAxis) = 0;

    virtual void animate(double deltaT) = 0;
    virtual Viewport getViewport() const = 0;
    virtual void resize(const Viewport& viewport) = 0;
};
} // namespace FilApp

#endif // FILAPP_IVIEW_HPP
