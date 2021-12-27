#ifndef FILAPP_FILAPPLICATION_HPP
#define FILAPP_FILAPPLICATION_HPP

#include <Core/Utils/Compiler.hpp>
#include <GraphicsInterface/AppConfig.hpp>
#include <GraphicsInterface/Application.hpp>
#include <GraphicsInterface/Window.hpp>
#include <GraphicsInterface/WindowConfig.hpp>
#include <cmath>
#include <filapp_export.h>
#include <memory>

namespace filament
{
class Engine;
}

namespace FilApp
{
class FilAppWindow;

class FILAPP_EXPORT FilApplication : public Application {
  public:
    static void init(const AppConfig& appConfig,
                     const WindowConfig& windowConfig);
    CORE_NODISCARD static FilApplication& get();

    FilApplication() = default;
    ~FilApplication() override;
    FilApplication(const FilApplication& application) = delete;
    FilApplication(FilApplication&& application) = delete;
    FilApplication& operator=(const FilApplication& application) = delete;
    FilApplication& operator=(FilApplication&& application) = delete;

    CORE_NODISCARD filament::Engine* getEngine();
    CORE_NODISCARD Window* getWindow() override;

    CORE_NODISCARD static double_t getDeltaT();

    void run();

  private:
    static std::unique_ptr<FilApplication> m_app;
    static double_t m_prevTimeStep;

    filament::Engine* m_engine = nullptr;

    std::unique_ptr<FilAppWindow> m_window;

    bool m_closeApp = false;
    AppConfig m_appConfig;
};
} // namespace FilApp

#endif // FILAPP_FILAPPLICATION_HPP
