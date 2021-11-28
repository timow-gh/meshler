#include "FilApp/FilAppView.hpp"
#include "FilAppConversion.hpp"
#include <FilApp/FilamentCoordinateSystem.hpp>
#include <camutils/Bookmark.h>
#include <filament/Options.h>
#include <filament/TransformManager.h>
#include <math/mat4.h>
#include <math/mathfwd.h>
#include <math/vec3.h>
#include <utility>

namespace FilApp
{
FilAppView::FilAppView(const ViewConfig& viewConfig,
                       filament::Renderer& renderer)
    : m_engine(renderer.getEngine()), m_name(viewConfig.name)
{
    m_filamentView = m_engine->createView();
    m_filamentView->setName(m_name.c_str());

    if (viewConfig.useFXAA)
    {
        m_filamentView->setAntiAliasing(filament::AntiAliasing::FXAA);
        m_filamentView->setSampleCount(viewConfig.fxaaSampleCount);
    }

    m_scene = m_engine->createScene();
    m_filamentView->setScene(m_scene);

    m_skybox = filament::Skybox::Builder()
                   .color(toFilamentVec(viewConfig.skyBoxColor))
                   .build(*m_engine);
    m_scene->setSkybox(m_skybox);

    utils::EntityManager& entityManager = utils::EntityManager::get();
    m_cameraEntity = entityManager.create();
    m_camera = m_engine->createCamera(m_cameraEntity);

    const filament::math::float3 eye = transformToFilamentVec(viewConfig.eye);
    const filament::math::float3 center =
        transformToFilamentVec(viewConfig.center);
    const filament::math::float3 up = transformToFilamentVec(viewConfig.up);

    m_camera->lookAt(eye, center, up);
    m_filamentView->setCamera(m_camera);

    filament::camutils::Mode cameraMode = calcCameraMode(viewConfig.cameraMode);
    if (cameraMode == filament::camutils::Mode::ORBIT)
        m_cameraManipulator = std::unique_ptr<CameraManipulator>(
            CameraManipulator::Builder()
                .orbitHomePosition(eye[0], eye[1], eye[2])
                .targetPosition(center[0], center[1], center[2])
                .upVector(up[0], up[1], up[2])
                .zoomSpeed(1.0)
                .build(cameraMode));
    else if (cameraMode == filament::camutils::Mode::FREE_FLIGHT)
        m_cameraManipulator = std::unique_ptr<CameraManipulator>(
            CameraManipulator::Builder()
                .flightStartPosition(eye[0], eye[1], eye[2])
                .flightMoveDamping(15.0)
                .build(cameraMode));
    else
        assert(false &&
               "Camera manipulator not "
               "implemented.");

    m_renderableCreator = FilAppRenderableCreator::create(m_engine);

    m_globalTrafoComponent = utils::EntityManager::get().create();
    auto& tcm = m_engine->getTransformManager();
    tcm.create(m_globalTrafoComponent);
    auto globalInstance = tcm.getInstance(m_globalTrafoComponent);
    tcm.setTransform(globalInstance, filCSToGlobalCS4());

    setViewport(viewConfig.viewport);
}

FilAppView::~FilAppView()
{
    utils::EntityManager& entityManager = utils::EntityManager::get();
    clearFilAppRenderables();
    entityManager.destroy(m_cameraEntity);
    m_engine->destroyCameraComponent(m_cameraEntity);
    m_engine->destroy(m_skybox);
    m_engine->destroy(m_filamentView);
    m_engine->destroy(m_scene);
    auto& tcm = utils::EntityManager::get();
    tcm.destroy(m_globalTrafoComponent);
}
void FilAppView::animate(double deltaT)
{
    for (const auto& animationCallBack: m_animationCallbacks)
        animationCallBack(deltaT);
}
filament::View* FilAppView::getFilamentView()
{
    return m_filamentView;
}
filament::Camera* FilAppView::getCamera()
{
    return m_camera;
}
FilAppView::CameraManipulator* FilAppView::getCameraManipulator()
{
    return m_cameraManipulator.get();
}
void FilAppView::configureOrthogonalProjection(float_t near,
                                               float_t far,
                                               float zoom)
{
    const float aspect = (float)m_viewport.width / (float)m_viewport.height;
    m_camera->setProjection(filament::Camera::Projection::ORTHO,
                            -aspect * zoom,
                            aspect * zoom,
                            -zoom,
                            zoom,
                            near,
                            far);
}
RenderableIdentifier
FilAppView::addRenderable(TriangleRenderable&& triangleRenderable)
{
    auto renderable =
        std::make_unique<TriangleRenderable>(std::move(triangleRenderable));

    auto id = addRenderable(m_renderableCreator.createBakedColorRenderable(
        renderable->getVertices(),
        renderable->getIndices(),
        filament::RenderableManager::PrimitiveType::TRIANGLES));

    m_triangleRenderables.emplace(id, std::move(renderable));

    return id;
}
RenderableIdentifier
FilAppView::addRenderable(PointRenderable&& pointRenderable)
{
    auto renderable =
        std::make_unique<PointRenderable>(std::move(pointRenderable));

    auto id = addRenderable(m_renderableCreator.createBakedColorRenderable(
        renderable->getVertices(),
        renderable->getIndices(),
        filament::RenderableManager::PrimitiveType::POINTS));

    m_pointRenderables.emplace(id, std::move(renderable));

    return id;
}
RenderableIdentifier FilAppView::addRenderable(LineRenderable&& lineREnderable)
{
    auto renderable =
        std::make_unique<LineRenderable>(std::move(lineREnderable));

    auto id = addRenderable(m_renderableCreator.createBakedColorRenderable(
        renderable->getVertices(),
        renderable->getIndices(),
        filament::RenderableManager::PrimitiveType::LINES));

    m_lineRenderables.emplace(id, std::move(renderable));
    return id;
}
std::vector<RenderableIdentifier> FilAppView::getRenderableIdentifiers() const
{
    std::vector<RenderableIdentifier> result;
    for (const auto& filAppRenderable: m_renderables)
        result.push_back(filAppRenderable.renderableEntity.getId());
    return result;
}
void FilAppView::removeRenderable(RenderableIdentifier id)
{
    eraseFromMap(m_pointRenderables, id);
    eraseFromMap(m_lineRenderables, id);
    eraseFromMap(m_triangleRenderables, id);
    auto iter =
        std::remove_if(m_renderables.begin(),
                       m_renderables.end(),
                       [id = id, scene = m_scene](const FilAppRenderable& item)
                       {
                           if (item.renderableEntity.getId() == id)
                           {
                               scene->remove(item.renderableEntity);
                               item.destroy();
                               return true;
                           }
                           return false;
                       });
    m_renderables.erase(iter, m_renderables.end());
}
void FilAppView::clearRenderables()
{
    clearFilAppRenderables();
}
void FilAppView::setUsePostprocessing(bool usePostProcessing)
{
    m_filamentView->setPostProcessingEnabled(usePostProcessing);
}
void FilAppView::addRotationAnimation(RenderableIdentifier renderableIdentifier,
                                      const Vec3& rotationAxis)
{
    m_animationCallbacks.emplace_back(
        [renderableIdentifier, engine = m_engine](double deltaT)
        {
            auto& tcm = engine->getTransformManager();
            tcm.setTransform(tcm.getInstance(utils::Entity::import(
                                 static_cast<int>(renderableIdentifier))),
                             filament::math::mat4f::rotation(
                                 deltaT * 0.4,
                                 filament::math::float3{0, 1, 0}));
        });
}
Viewport FilAppView::getViewport() const
{
    return {m_viewport.left,
            m_viewport.bottom,
            m_viewport.width,
            m_viewport.height};
}
void FilAppView::setViewport(const Viewport& viewport)
{
    m_viewport = calcViewport(viewport);
    m_filamentView->setViewport(m_viewport);
    configureOrthogonalProjection(m_near, m_far, m_orthogonalCameraZoom);
    if (m_cameraManipulator)
        m_cameraManipulator->setViewport(static_cast<int>(viewport.width),
                                         static_cast<int>(viewport.height));
}
void FilAppView::setCamera(filament::Camera* camera)
{
    m_filamentView->setCamera(camera);
}
void FilAppView::resize(const Viewport& viewport)
{
    setViewport(viewport);
}
void FilAppView::mouseDown(const MouseDownEvent& mouseDownEvent)
{
    if (m_cameraManipulator)
        m_cameraManipulator->grabBegin(static_cast<int>(mouseDownEvent.pos.x),
                                       static_cast<int>(mouseDownEvent.pos.y),
                                       mouseDownEvent.button == 3);
    for (auto listener: m_inputEventListener)
        listener->mouseDown(mouseDownEvent);
}
void FilAppView::mouseUp(const MouseUpEvent& mouseUpEvent)
{
    if (m_cameraManipulator)
        m_cameraManipulator->grabEnd();
    for (auto listener: m_inputEventListener)
        listener->mouseUp(mouseUpEvent);
}
void FilAppView::mouseMoved(const MouseMovedEvent& mouseMovedEvent)
{
    if (m_cameraManipulator)
        m_cameraManipulator->grabUpdate(
            static_cast<int>(mouseMovedEvent.pos.x),
            static_cast<int>(mouseMovedEvent.pos.y));
    for (auto listener: m_inputEventListener)
        listener->mouseMoved(mouseMovedEvent);
}
void FilAppView::mouseWheel(const MouseWheelEvent& mouseWheelEvent)
{
    if (m_cameraManipulator)
    {
        m_cameraManipulator->scroll(0, 0, mouseWheelEvent.x);
        m_orthogonalCameraZoom -= mouseWheelEvent.x * 1.5;
        configureOrthogonalProjection(m_near, m_far, m_orthogonalCameraZoom);
    }

    for (auto listener: m_inputEventListener)
        listener->mouseWheel(mouseWheelEvent);
}
void FilAppView::keyDown(const KeyDownEvent& keyDownEvent)
{
    if (m_cameraManipulator)
    {
        CameraManipulator::Key key;
        if (manipulatorKeyFromKeycode(keyDownEvent.sdlScancode, key))
            m_cameraManipulator->keyDown(key);
    }
    for (auto listener: m_inputEventListener)
        listener->keyDown(keyDownEvent);
}
void FilAppView::keyUp(const KeyUpEvent& keyUpEvent)
{
    if (m_cameraManipulator)
    {
        CameraManipulator::Key key;
        if (manipulatorKeyFromKeycode(keyUpEvent.sdlScancode, key))
        {
            m_cameraManipulator->keyDown(key);
        }
    }
    for (auto listener: m_inputEventListener)
        listener->keyUp(keyUpEvent);
}
bool FilAppView::manipulatorKeyFromKeycode(
    SDL_Scancode scancode,
    filament::camutils::Manipulator<float_t>::Key& key)
{
    switch (scancode)
    {
    case SDL_SCANCODE_W: key = CameraManipulator::Key::FORWARD; return true;
    case SDL_SCANCODE_A: key = CameraManipulator::Key::LEFT; return true;
    case SDL_SCANCODE_S: key = CameraManipulator::Key::BACKWARD; return true;
    case SDL_SCANCODE_D: key = CameraManipulator::Key::RIGHT; return true;
    case SDL_SCANCODE_E: key = CameraManipulator::Key::UP; return true;
    case SDL_SCANCODE_Q: key = CameraManipulator::Key::DOWN; return true;
    default: return false;
    }
}
RenderableIdentifier
FilAppView::addRenderable(const FilAppRenderable& filAppRenderable)
{
    m_renderables.emplace_back(filAppRenderable);
    auto entity = m_renderables.back().renderableEntity;
    m_scene->addEntity(entity);

    auto& tcm = m_engine->getTransformManager();
    auto globalInstance = tcm.getInstance(m_globalTrafoComponent);
    auto renderableInstance = tcm.getInstance(entity);
    tcm.setParent(renderableInstance, globalInstance);

    return m_renderables.back().renderableEntity.getId();
}
void FilAppView::clearFilAppRenderables()
{
    for (auto& renderable: m_renderables)
    {
        m_scene->remove(renderable.renderableEntity);
        renderable.destroy();
    }
    m_renderables.clear();
}

} // namespace FilApp