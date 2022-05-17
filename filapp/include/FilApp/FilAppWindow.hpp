#ifndef FILAPP_FILAPPWINDOW_HPP
#define FILAPP_FILAPPWINDOW_HPP

#include <FilApp/FilAppCameraView.hpp>
#include <FilApp/FilApplication.hpp>
#include <Graphics/InputEvents/InputEventDispatcher.hpp>
#include <Graphics/InputEvents/MouseButtonEvent.hpp>
#include <Graphics/InputEvents/RayPickEventDispatcher.hpp>
#include <Graphics/Window.hpp>
#include <Graphics/WindowConfig.hpp>
#include <SDL_video.h>
#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Graphics
{

class FilAppWindow : public Window {
    SDL_Window* m_sdlWindow = nullptr;
    Window::WindowId m_windowId{0};
    filament::Engine* m_engine = nullptr;
    filament::Renderer* m_renderer = nullptr;
    filament::Engine::Backend m_backend = filament::Engine::Backend::DEFAULT;
    filament::SwapChain* m_swapChain = nullptr;

    std::unique_ptr<FilAppCameraView> m_mainView{nullptr};
    std::vector<std::unique_ptr<FilAppCameraView>> m_views;

    std::uint32_t m_width = 0;
    std::uint32_t m_height = 0;
    std::size_t m_lastX = 0;
    std::size_t m_lastY = 0;

    std::unordered_map<SDL_Scancode, FilAppCameraView*> m_keyEventTarget;

    InputEventDispatcher m_inputEventDispatcher;
    RayPickEventDispatcher m_rayPickEventDispatcher;

  public:
    FilAppWindow(const WindowConfig& windowConfig, filament::Engine* engine);
    FilAppWindow(FilAppWindow&& window) = default;
    FilAppWindow& operator=(FilAppWindow&& window) = default;
    ~FilAppWindow() override;

    CORE_NODISCARD InputEventDispatcher& getInputEventDispatcher() override;
    CORE_NODISCARD RayPickEventDispatcher& getRayPickEventDispatcher() override;

    void event(const MouseButtonEvent& mouseButtonEvent);
    void event(const MouseMoveEvent& mouseMoveEvent);
    void event(const KeyEvent& keyEventr);
    void mouseWheel(float_t x, double_t deltaT);

    void resize();
    void animate(double_t deltaT);
    void render();

    CORE_NODISCARD View* getMainIView() override;
    CORE_NODISCARD std::vector<View*> getIViews() override;

    WindowId getIWindowId() override;

    CORE_NODISCARD SDL_Window* getSdlWindow() const;
    CORE_NODISCARD uint32_t getWidth() const;
    CORE_NODISCARD uint32_t getHeight() const;

    CORE_NODISCARD filament::Renderer* getRenderer();
    CORE_NODISCARD filament::SwapChain* getSwapChain();

    CORE_NODISCARD filament::math::int2 fixupMouseCoordinatesForHdpi(uint32_t x, uint32_t y) const;

  private:
    CORE_NODISCARD Viewport calcWindowViewport();
};

bool intersects(const Viewport& viewport, size_t x, size_t y);
} // namespace Graphics

#endif // FILAPP_FILAPPWINDOW_HPP
