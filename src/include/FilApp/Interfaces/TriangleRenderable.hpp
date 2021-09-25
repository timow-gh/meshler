#ifndef FILAPP_TRIANGLERENDERABLE_HPP
#define FILAPP_TRIANGLERENDERABLE_HPP

#include "Vertex.hpp"
#include <filapp_export.h>
#include <utility>
#include <vector>

namespace FilApp
{
class FILAPP_EXPORT TriangleRenderable
{
  public:
    TriangleRenderable() = default;
    TriangleRenderable(std::vector<Vertex>&& vertices,
                       std::vector<uint16_t>&& indices)
        : m_vertices(std::move(vertices)), m_indices(std::move(indices))
    {
    }

    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<uint16_t>& getIndices() const { return m_indices; }

  private:
    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;
};
} // namespace FilApp

#endif // FILAPP_TRIANGLERENDERABLE_HPP
