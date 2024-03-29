#ifndef GRAPHICS_RGBACOLOR_HPP
#define GRAPHICS_RGBACOLOR_HPP

#include <Core/Types/TArray.hpp>
#include <cmath>
#include <cstdint>

namespace Graphics
{

template <typename T>
class RGBAColor {
  Core::TArray<T, 4> m_values;

public:
  explicit RGBAColor(const std::array<T, 3>& values)
      : m_values(values[0], values[1], values[2], 1.0f)
  {
  }

  RGBAColor(T red, T green, T blue)
      : m_values({red, green, blue, 1.0f})
  {
  }
  RGBAColor(T red, T green, T blue, T alpha)
      : m_values({red * alpha, green * alpha, blue * alpha, alpha})
  {
  }

  static RGBAColor<float_t> create(std::uint32_t red, std::uint32_t green, std::uint32_t blue, float_t alpha)
  {
    auto sRGB_to_linear = [](float color) -> float_t
    { return color <= 0.04045f ? color / 12.92f : std::pow((color + 0.055f) / 1.055f, 2.4f); };
    float_t sum = 255.0f; // red + green + blue; //
    return {sRGB_to_linear(static_cast<float_t>(red) / sum) * alpha,
            sRGB_to_linear(static_cast<float_t>(green) / sum) * alpha,
            sRGB_to_linear(static_cast<float_t>(blue) / sum) * alpha,
            alpha};
  }

  static RGBAColor<float_t> create(std::uint32_t red, std::uint32_t green, std::uint32_t blue)
  {
    float_t alpha = 1.0f;
    return RGBAColor<float_t>::create(red, green, blue, alpha);
  }

  CORE_NODISCARD T getRed() const { return m_values[0]; }
  CORE_NODISCARD T getGreen() const { return m_values[1]; }
  CORE_NODISCARD T getBlue() const { return m_values[2]; }
  CORE_NODISCARD T getAlpha() const { return m_values[3]; }
};

} // namespace Graphics

#endif // GRAPHICS_RGBACOLOR_HPP
