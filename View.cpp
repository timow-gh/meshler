#include "View.h"
#include "SDL_timer.h"
#include <utils/EntityManager.h>

namespace FilApp
{
View::View(filament::Renderer& renderer,
           const std::string& name,
           const filament::Viewport& viewport,
           filament::camutils::Mode cameraMode)
    : m_engine(renderer.getEngine()), m_name(name)
{
    m_filamentView = m_engine->createView();
    m_filamentView->setName(m_name.c_str());

    utils::EntityManager& entityManager = utils::EntityManager::get();
    m_cameraEntity = entityManager.create();
    m_camera = m_engine->createCamera(m_cameraEntity);
    m_camera->setExposure(16.0f, 1 / 125.0f, 100.0f);
    m_camera->lookAt({0, 0, 10}, {0, 0, 0}, {1, 0, 0});
    m_filamentView->setCamera(m_camera);

    m_cameraManipulator =
        std::unique_ptr<CameraManipulator>(CameraManipulator::Builder()
                                               .targetPosition(0, 0, 0)
                                               .flightMoveDamping(15.0)
                                               .build(cameraMode));

    setViewport(viewport);
}
View::~View()
{
    utils::EntityManager& entityManager = utils::EntityManager::get();
    entityManager.destroy(m_cameraEntity);
    m_engine->destroy(m_filamentView);
}
void View::setViewport(const filament::Viewport& viewport)
{
    m_viewport = viewport;
    m_filamentView->setViewport(m_viewport);
    configureCameraProjection();
    if (m_cameraManipulator)
        m_cameraManipulator->setViewport(viewport.width, viewport.height);
}
void View::setCamera(filament::Camera* camera)
{
    m_filamentView->setCamera(camera);
}
void View::setCameraManipulator(View::CameraManipulator* cameraManipulator)
{
    m_cameraManipulator.reset();
    m_cameraManipulator = std::unique_ptr<CameraManipulator>(cameraManipulator);
}
void View::addViewListener(ViewListener* viewListener)
{
    m_viewListener.push_back(viewListener);
}
void View::clearViewListener()
{
    m_viewListener.clear();
}
void View::mouseDown(int button, ssize_t x, ssize_t y)
{
    if (m_cameraManipulator)
        m_cameraManipulator->grabBegin(x, y, button == 3);
    for (auto listener: m_viewListener)
        listener->mouseDownEvent(
            MouseDownEvent(button, x, y, SDL_GetPerformanceCounter()));
}
void View::mouseUp(ssize_t x, ssize_t y)
{
    if (m_cameraManipulator)
        m_cameraManipulator->grabEnd();
    for (auto listener: m_viewListener)
        listener->mouseUpEvent(MouseUpEvent(x, y, SDL_GetPerformanceCounter()));
}
void View::mouseMoved(ssize_t x, ssize_t y)
{
    if (m_cameraManipulator)
        m_cameraManipulator->grabUpdate(x, y);
    for (auto listener: m_viewListener)
        listener->mouseMovedEvent(
            MouseMovedEvent(x, y, SDL_GetPerformanceCounter()));
}
void View::mouseWheel(ssize_t x)
{
    if (m_cameraManipulator)
        m_cameraManipulator->scroll(0, 0, x);
    for (auto listener: m_viewListener)
        listener->mouseScrollEvent(
            MouseScrollEvent(x, SDL_GetPerformanceCounter()));
}
filament::View* View::getFilamentView()
{
    return m_filamentView;
}
filament::Camera* View::getCamera()
{
    return m_camera;
}
View::CameraManipulator* View::getCameraManipulator()
{
    return m_cameraManipulator.get();
}
const filament::Viewport& View::getViewport()
{
    return m_viewport;
}
void View::keyDown(SDL_Scancode scancode)
{
    if (m_cameraManipulator)
    {
        CameraManipulator::Key key;
        if (manipulatorKeyFromKeycode(scancode, key))
            m_cameraManipulator->keyDown(key);
    }
}
void View::keyUp(SDL_Scancode scancode)
{
    if (m_cameraManipulator)
    {
        CameraManipulator::Key key;
        if (manipulatorKeyFromKeycode(scancode, key))
        {
            m_cameraManipulator->keyDown(key);
        }
    }
}
void View::resize(const filament::Viewport& viewport)
{
    setViewport(viewport);
}
bool View::manipulatorKeyFromKeycode(
    SDL_Scancode scancode,
    filament::camutils::Manipulator<float_t>::Key& key) const
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
void View::configureCameraProjection()
{
    constexpr float ZOOM = 1.5f;
    const float aspect = (float)m_viewport.width / m_viewport.height;
    m_camera->setProjection(filament::Camera::Projection::ORTHO,
                            -aspect * ZOOM,
                            aspect * ZOOM,
                            -ZOOM,
                            ZOOM,
                            0,
                            1);
}
} // namespace FilApp