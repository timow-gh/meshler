#include <FilApp/FilAppConversion.hpp>
#include <FilApp/FilAppWindow.hpp>
#include <FilAppInterface/InputEvents/KeyUpEvent.hpp>
#include <NativeWindowHelper.hpp>
#include <SDL_video.h>
#include <utils/EntityManager.h>

namespace FilApp
{

FilAppWindow::FilAppWindow(const WindowConfig& windowConfig,
                           FilApplication* application)
{
    m_application = application;

    uint32_t windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;
    if (windowConfig.isResizable)
        windowFlags |= SDL_WINDOW_RESIZABLE;
    if (windowConfig.isHeadless)
        windowFlags |= SDL_WINDOWEVENT_HIDDEN;

    const int x = SDL_WINDOWPOS_CENTERED;
    const int y = SDL_WINDOWPOS_CENTERED;
    m_sdlWindow = SDL_CreateWindow(windowConfig.windowName.c_str(),
                                   x,
                                   y,
                                   static_cast<int>(windowConfig.width),
                                   static_cast<int>(windowConfig.height),
                                   windowFlags);

    m_windowId = SDL_GetWindowID(m_sdlWindow);

    if (windowConfig.isHeadless)
    {
        m_width = windowConfig.width;
        m_height = windowConfig.height;
        m_swapChain =
            m_application->getEngine()->createSwapChain(m_width, m_height);
    }
    else
    {
        m_backend = m_application->getEngine()->getBackend();

        void* nativeWindow = ::getNativeWindow(m_sdlWindow);
        void* nativeSwapChain = nativeWindow;
        m_swapChain =
            m_application->getEngine()->createSwapChain(nativeSwapChain);
    }

    m_renderer = m_application->getEngine()->createRenderer();

    ViewConfig viewConfig;
    viewConfig.name = "MainView";
    viewConfig.viewport = calcWindowViewport();
    m_views.emplace_back(std::make_unique<FilAppView>(viewConfig, *m_renderer));
    m_mainView = m_views.back().get();
}

void FilAppWindow::event(const MouseButtonEvent& evt)
{
    m_mainView->event(evt);
    for (const auto& view: m_views)
    {
        if (intersects(view->getViewport(), evt.x, evt.y))
        {
            view->event(evt);
            break;
        }
    }
}

void FilAppWindow::event(const MouseMoveEvent& evt)
{
    m_mainView->event(evt);
    for (const auto& view: m_views)
    {
        if (intersects(view->getViewport(), evt.x, evt.y))
        {
            view->event(evt);
            break;
        }
    }
    m_lastX = evt.x;
    m_lastY = evt.y;
}

void FilAppWindow::event(const KeyUpEvent& keyUpEvent)
{
    auto& eventTargetView = m_keyEventTarget[keyUpEvent.sdlScancode];
    if (!eventTargetView)
        return;
    eventTargetView->event(
        KeyUpEvent(keyUpEvent.sdlScancode, keyUpEvent.deltaT));
    eventTargetView = nullptr;
}

void FilAppWindow::event(const KeyDownEvent& keyDownEvent)
{
    auto& eventTarget = m_keyEventTarget[keyDownEvent.sdlScancode];

    // event events can be sent multiple times per key (for key repeat)
    // If this key is already down, do nothing.
    if (eventTarget)
        return;

    // Decide which view will get this key's corresponding event event.
    // If we're currently in a mouse grap session, it should be the mouse grab's
    // target view. Otherwise, it should be whichever view we're currently
    // hovering over.
    m_mainView->event(keyDownEvent);

    for (auto const& view: m_views)
    {
        if (intersects(view->getViewport(), m_lastX, m_lastY))
        {
            view->event(keyDownEvent);
            break;
        }
    }
}

void FilAppWindow::mouseWheel(float_t x, double_t deltaT)
{
    m_mainView->event(MouseWheelEvent(x, deltaT));
    for (auto const& view: m_views)
    {
        if (intersects(view->getViewport(), m_lastX, m_lastY))
        {
            view->event(MouseWheelEvent(x, deltaT));
            break;
        }
    }
}

FilAppWindow::~FilAppWindow()
{
    SDL_DestroyWindow(m_sdlWindow);
}

std::vector<View*> FilAppWindow::getIViews()
{
    std::vector<View*> views;
    for (const auto& filappview: m_views)
        views.push_back(filappview.get());
    return views;
}

Window::WindowId FilAppWindow::getIWindowId()
{
    return m_windowId;
}

SDL_Window* FilAppWindow::getSdlWindow() const
{
    return m_sdlWindow;
}

uint32_t FilAppWindow::getWidth() const
{
    return m_width;
}

uint32_t FilAppWindow::getHeight() const
{
    return m_height;
}

FilAppView* FilAppWindow::getMainFilAppView()
{
    return m_mainView;
}

filament::Renderer* FilAppWindow::getRenderer()
{
    return m_renderer;
}

filament::SwapChain* FilAppWindow::getSwapChain()
{
    return m_swapChain;
}

filament::math::int2 FilAppWindow::fixupMouseCoordinatesForHdpi(uint32_t x,
                                                                uint32_t y) const

{
    int dw, dh, ww, wh;
    SDL_GL_GetDrawableSize(m_sdlWindow, &dw, &dh);
    SDL_GetWindowSize(m_sdlWindow, &ww, &wh);
    x = x * dw / ww;
    y = y * dh / wh;
    y = m_height - y;
    return filament::math::int2{x, y};
}

void FilAppWindow::resize()
{
    if (m_sdlWindow)
        m_mainView->resize(calcWindowViewport());
}

View* FilAppWindow::getMainIView()
{
    return m_mainView;
}

void FilAppWindow::render()
{
    if (m_renderer->beginFrame(getSwapChain()))
    {
        for (auto const& view: m_views)
            m_renderer->render(view->getFilamentView());
        m_renderer->endFrame();
    }
}

void FilAppWindow::animate(double_t deltaT)
{
    if (auto mainViewManip = m_mainView->getCameraManipulator())
    {
        filament::math::float3 eye;
        filament::math::float3 center;
        filament::math::float3 up;
        mainViewManip->update(static_cast<float_t>(deltaT));
        mainViewManip->getLookAt(&eye, &center, &up);
        m_mainView->getCamera()->lookAt(eye, center, up);
    }

    for (auto& filappview: m_views)
        filappview->animate(deltaT);
}

bool intersects(const Viewport& viewport, size_t x, size_t y)
{
    if (x >= viewport.left && x < viewport.left + viewport.width)
        if (y >= viewport.bottom && y < viewport.bottom + viewport.height)
            return true;
    return false;
}

Viewport FilAppWindow::calcWindowViewport()
{
    SDL_GL_GetDrawableSize(m_sdlWindow, (int*)&m_width, (int*)&m_height);
    return {0,
            0,
            static_cast<uint32_t>(m_width),
            static_cast<uint32_t>(m_height)};
}
} // namespace FilApp