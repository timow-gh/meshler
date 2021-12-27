#include <Core/Types/TArray.hpp>
#include <FilApp/FilApplication.hpp>
#include <GraphicsInterface/View.hpp>
#include <GraphicsInterface/Window.hpp>
#include <FlowMesh/FlowMeshController.hpp>
#include <FlowMesh/FlowMeshModel.hpp>
#include <FlowMesh/FlowMeshPresenter.hpp>
#include <FlowMesh/GeometryElements/FlowMeshSegments.hpp>
#include <FlowMesh/GeometryElements/FlowMeshSphere.hpp>
#include <FlowMesh/Interactors/PlacingInteractor.hpp>
#include <Geometry/Segment.hpp>
#include <Geometry/Sphere.hpp>
#include <LinAl/LinearAlgebra.hpp>

using namespace Graphics;
using namespace FlowMesh;
using namespace Geometry;
using namespace LinAl;

void createGridSegments(FlowMeshModel& fMModel)
{
    Core::TVector<Geometry::Segment3d> xSegs;
    Core::TVector<Geometry::Segment3d> ySegs;
    constexpr int32_t MIN = -5;
    constexpr int32_t MAX = 5;
    constexpr std::size_t SEG_COUNT = MAX - MIN + 1;
    constexpr double_t LENGTH_HALF = static_cast<double_t>(MAX - MIN) / 2.0;
    for (int32_t i = 0; i < SEG_COUNT; ++i)
    {
        xSegs.push_back(Geometry::Segment3d{
            {static_cast<double>(MIN + i), -LENGTH_HALF, 0},
            {static_cast<double>(MIN + i), LENGTH_HALF, 0}});
        ySegs.push_back(Geometry::Segment3d{
            {-LENGTH_HALF, static_cast<double>(MIN + i), 0},
            {LENGTH_HALF, static_cast<double>(MIN + i), 0}});
    }
    fMModel.add(FlowMeshSegments(xSegs, newFGuid()));
    fMModel.add(FlowMeshSegments(ySegs, newFGuid()));
}

void createSpheres(FlowMeshModel& fMModel)
{
    FGuid sphereToRemove;

    constexpr int32_t MINMAX = 1;
    constexpr double_t RADIUS = 0.5;
    constexpr double_t DIST = 2.0;
    for (int32_t i{-MINMAX}; i <= MINMAX; ++i)
    {
        for (int32_t j{-MINMAX}; j <= MINMAX; ++j)
        {
            FGuid id = newFGuid();
            if (i == 0 && j == 0)
                sphereToRemove = id;

            fMModel.add(FlowMeshSphere(
                Sphere<double_t>(Vec3d{static_cast<double>(i) * DIST,
                                       static_cast<double>(j) * DIST,
                                       0},
                                 RADIUS),
                id));
        }
    }

    fMModel.remove(sphereToRemove);
}

void createCones(FlowMeshModel& fmModel)
{
    // x
    fmModel.add(FlowMeshCone(
        Geometry::Cone<double_t>(
            Segment3d{LinAl::Vec3d{0, 0, 0}, LinAl::Vec3d{0.5, 0, 0}},
            0.1),
        newFGuid()));

    // y
    fmModel.add(FlowMeshCone(
        Geometry::Cone<double_t>(
            Segment3d{LinAl::Vec3d{0, 0, 0}, LinAl::Vec3d{0, 0.5, 0}},
            0.1),
        newFGuid()));
    fmModel.add(FlowMeshCone(
        Geometry::Cone<double_t>(
            Segment3d{LinAl::Vec3d{0, 0.5, 0}, LinAl::Vec3d{0, 1.0, 0}},
            0.1),
        newFGuid()));

    // z
    fmModel.add(FlowMeshCone(
        Geometry::Cone<double_t>(
            Segment3d{LinAl::Vec3d{0, 0, 0}, LinAl::Vec3d{0, 0, 0.5}},
            0.1),
        newFGuid()));
    fmModel.add(FlowMeshCone(
        Geometry::Cone<double_t>(
            Segment3d{LinAl::Vec3d{0, 0, 0.5}, LinAl::Vec3d{0, 0, 1.0}},
            0.1),
        newFGuid()));
    fmModel.add(FlowMeshCone(
        Geometry::Cone<double_t>(
            Segment3d{LinAl::Vec3d{0, 0, 1.0}, LinAl::Vec3d{0, 0, 1.5}},
            0.1),
        newFGuid()));
}

int main()
{
    AppConfig appConfig{};
    appConfig.backendMode = BackendMode::VULKAN;
    appConfig.eventPollingMode = EventPollingMode::WAIT_EVENTS;

    FilApplication::init(appConfig, WindowConfig());
    FilApplication& app = FilApplication::get();

    Window* mainWindow = app.getWindow();
    View* mainView = mainWindow->getMainIView();

    FlowMeshModel flowMeshModel;

    FlowMeshController meshController =
        FlowMeshController(mainView,
                           &flowMeshModel,
                           std::make_unique<PlacingInteractor>(&flowMeshModel));

    FlowMeshPresenter flowMeshPresenter;
    flowMeshPresenter.setMainView(mainView);

    flowMeshModel.setFlowMeshPresenter(&flowMeshPresenter);

    createGridSegments(flowMeshModel);
    createSpheres(flowMeshModel);
    createCones(flowMeshModel);

//    constexpr double_t radius = 0.5;
//    constexpr Vec3d connectVec = LinAl::Vec3d{0, 0, 4};
//    constexpr Segment3d cylinderSeg = {LinAl::Vec3d{0, 0, 1}, connectVec};
//    constexpr Segment3d coneSeg = {connectVec, LinAl::Vec3d{0, 0, 6}};
//    flowMeshModel.add(
//        FlowMeshCylinder(Geometry::Cylinder<double_t>(cylinderSeg, radius),
//                         newFGuid()));
//    flowMeshModel.add(
//        FlowMeshCone(Geometry::Cone<double_t>(coneSeg, radius), newFGuid()));

    app.run();
}