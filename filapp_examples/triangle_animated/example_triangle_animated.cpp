#include <FilApp/FilApplication.hpp>
#include <Graphics/Renderables/TriangleRenderable.hpp>
#include <Graphics/Vec.hpp>
#include <Graphics/View.hpp>

using namespace Graphics;

int main()
{
  AppConfig appConfig;
  appConfig.eventPollingMode = Graphics::EventPollingMode::WAIT_EVENTS;

  std::shared_ptr<Graphics::GraphicsApp> graphicsApp = FilApp::FilApplication::getFilApp(appConfig, WindowConfig());

  Window& mainWindow = graphicsApp->getWindow();
  View* mainView = mainWindow.getMainIView();

  std::vector<Vertex> vertices = {
      {{0, 0, 0}, 0xffff0000u},
      {{1, 0, 0}, 0xff00ff00u},
      {{0, 0, 1}, 0xff0000ffu},
  };
  auto renderableId = mainView->addRenderable(TriangleRenderable(std::move(vertices), {0, 1, 2}));
  mainView->addRotationAnimation(renderableId, Vec3{0.0f, 1.0f, 0.0f});

  graphicsApp->run();

  return 0;
}