#ifndef GRAPHICS_VIEWPORT_HPP
#define GRAPHICS_VIEWPORT_HPP

#include <cstdint>

namespace Graphics
{
struct Viewport
{
  uint32_t left;
  uint32_t bottom;
  uint32_t width;
  uint32_t height;

  Viewport()
      : left(0)
      , bottom(0)
      , width(800)
      , height(640)
  {
  }
  Viewport(uint32_t left, uint32_t bottom, uint32_t width, uint32_t height)
      : left(left)
      , bottom(bottom)
      , width(width)
      , height(height)
  {
  }
};
} // namespace Graphics

#endif // GRAPHICS_VIEWPORT_HPP
