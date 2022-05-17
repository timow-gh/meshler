#ifndef FILAPP_FILAPPRENDERABLEBUFFERS_HPP
#define FILAPP_FILAPPRENDERABLEBUFFERS_HPP

#include <FilApp/FilAppRenderable.hpp>

namespace FilApp
{
void createVertexBuffer(FilAppRenderable* filAppRenderable,
                        const std::vector<Graphics::Vertex>& vertices);

void createVertexBuffer(FilAppRenderable* filAppRenderable,
                        Graphics::PointRenderable* pointRenderable,
                        std::size_t VERTEX_SIZE);

void createIndicesBuffer(FilAppRenderable* filAppRenderable, const std::vector<uint16_t>& indices);
} // namespace FilApp

#endif // FILAPP_FILAPPRENDERABLEBUFFERS_HPP
