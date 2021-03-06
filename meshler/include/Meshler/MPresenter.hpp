#ifndef MESHLER_MPRESENTER_HPP
#define MESHLER_MPRESENTER_HPP

#include <Core/Types/TMap.hpp>
#include <Graphics/Renderables/RendereableId.hpp>
#include <Meshler/MGuid.hpp>
#include <Meshler/MModelEventListener.hpp>
#include <Meshler/PresenterConfig.hpp>

namespace Graphics
{
class View;
class GraphicsController;
class InputEventListener;
class RayPickEventListener;
} // namespace Graphics

namespace Meshler
{
class MSphere;
class MCone;
class MCylinder;
class MSegments;
class MCuboid;
class MGrid;

class MPresenter : public MModelEventListener {
    std::reference_wrapper<Graphics::View> m_view;
    PresenterConfig m_presenterConfig{};

    Core::TMap<FGuid, Core::TVector<Graphics::RenderableId>> m_fGuidRenderableMapping;

  public:
    explicit MPresenter(Graphics::View& mainView);

    void registerListener(Graphics::GraphicsController* meshlerController);
    void removeListener(Graphics::GraphicsController* meshlerController);

    void registerInputEventListener(Graphics::InputEventListener* inputEventListener);
    void removeInputEventListener(Graphics::InputEventListener* inputEventListener);

    void registerRayPickEventListener(Graphics::RayPickEventListener* rayPickEventListener);
    void removeRayPickEventListener(Graphics::RayPickEventListener* rayPickEventListener);

    void onAdd(const MSphere& meshlerSphere) override;
    void onAdd(const MCone& meshlerCone) override;
    void onAdd(const MCylinder& meshlerCylinder) override;
    void onAdd(const MSegments& meshlerSegments) override;
    void onAdd(const MCuboid& meshlerCuboid) override;
    void onAdd(const MGrid& meshlerGrid) override;

    void onRemove(const FGuid& fGuid) override;

    void onUpdate(const MCylinder& meshlerCylinder) override;
    void onUpdate(const MCone& meshlerCone) override;
    void onUpdate(const MSegments& meshlerSegments) override;
    void onUpdate(const MSphere& meshlerSphere) override;
    void onUpdate(const MCuboid& meshlerCuboid) override;
    void onUpdate(const MGrid& meshlerGrid) override;

    void onPositionChanged(const PositionEvent& positionEvent) override;

  private:
    template <typename TGeomElement>
    void onUpdateImpl(const TGeomElement& elem)
    {
        onRemove(elem.getFGuid());
        onAdd(elem);
    }

    template <typename TGeomElement>
    void onAddImpl(const TGeomElement& geomElement);
};

} // namespace Meshler

#endif // MESHLER_MPRESENTER_HPP
