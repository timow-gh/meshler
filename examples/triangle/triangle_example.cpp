#include "../ExampleTriangle.hpp"
#include "FilApp/Window.h"
#include <filament/TransformManager.h>

int main()
{
    auto windowConfig = WindowConfig();
    windowConfig.cameraMode = filament::camutils::Mode::ORBIT;

    Application::init(AppConfig(), windowConfig);

    Window* window = Application::get().getWindow();
    auto mainView = window->getMainView();
    mainView->getFilamentView()->setPostProcessingEnabled(false);

    auto renderable = createTriangle(Application::get().getEngine(),
                                     TRIANGLE_VERTICES,
                                     TRIANGLE_INDICES);

    mainView->addRenderable(renderable);

    Application::get().run();
    return 0;
}