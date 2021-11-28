#ifndef FILAPP_IVIEW_HPP
#define FILAPP_IVIEW_HPP

#include "IInputEventDispatcher.hpp"
#include "IInputListener.hpp"
#include "Viewport.hpp"
#include <FilAppInterfaces/Renderables/LineRenderable.hpp>
#include <FilAppInterfaces/Renderables/PointRenderable.hpp>
#include <FilAppInterfaces/Renderables/RendereableIdentifier.hpp>
#include <FilAppInterfaces/Renderables/TriangleRenderable.hpp>
#include <FilAppInterfaces/Vec.hpp>

namespace FilApp
{

class  IView
    : public IInputListener
    , public IInputEventDispatcher
{
  public:
    ~IView() override = default;

    // clang-format off
    virtual auto addRenderable(TriangleRenderable&& renderable) -> RenderableIdentifier = 0;
    virtual auto addRenderable(PointRenderable&& renderable) -> RenderableIdentifier = 0;
    virtual auto addRenderable(LineRenderable&& renderable) -> RenderableIdentifier = 0;
    [[nodiscard]] virtual auto getRenderableIdentifiers() const -> std::vector<RenderableIdentifier> = 0;
    virtual void removeRenderable(RenderableIdentifier renderableIdentifier) = 0;
    virtual void clearRenderables() = 0;
    // clang-format on

    virtual void setUsePostprocessing(bool usePostProcessing) = 0;

    virtual void addRotationAnimation(RenderableIdentifier renderableIdentifier,
                                      const Vec3& rotationAxis) = 0;

    virtual void animate(double deltaT) = 0;
    [[nodiscard]] virtual Viewport getViewport() const = 0;
    virtual void resize(const Viewport& viewport) = 0;
};
} // namespace FilApp

#endif // FILAPP_IVIEW_HPP