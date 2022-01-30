#ifndef FILAPP_PRESENTERCONFIG_HPP
#define FILAPP_PRESENTERCONFIG_HPP

namespace FlowMesh
{
struct PresenterConfig
{
    std::size_t spherePolarCount{16};
    std::size_t sphereAzimuthCount{20};
    std::size_t coneAzimuthCount{10};
    std::size_t cylinderAzimuthCount{10};

    std::uint32_t faceColor{0xFFc0bfbb};
    std::uint32_t lineColor{0xFF000000};
};
} // namespace FlowMesh

#endif // FILAPP_PRESENTERCONFIG_HPP