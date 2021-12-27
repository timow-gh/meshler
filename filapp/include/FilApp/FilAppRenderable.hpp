#ifndef FILAPP_FILAPPRENDERABLE_HPP
#define FILAPP_FILAPPRENDERABLE_HPP

#include <Core/Utils/Compiler.hpp>
#include <GraphicsInterface/Renderables/LineRenderable.hpp>
#include <GraphicsInterface/Renderables/PointRenderable.hpp>
#include <GraphicsInterface/Renderables/RendereableId.hpp>
#include <GraphicsInterface/Renderables/TriangleRenderable.hpp>
#include <GraphicsInterface/Vertex.hpp>
#include <filament/Engine.h>
#include <filament/IndexBuffer.h>
#include <filament/Material.h>
#include <filament/MaterialInstance.h>
#include <filament/RenderableManager.h>
#include <filament/VertexBuffer.h>
#include <filapp_export.h>
#include <memory>
#include <utils/EntityManager.h>

namespace FilApp
{
struct FilAppRenderable
{
    filament::Engine* engine = nullptr;
    filament::VertexBuffer* vb = nullptr;
    filament::IndexBuffer* ib = nullptr;
    filament::Material* mat = nullptr;
    filament::MaterialInstance* matInstance = nullptr;
    utils::Entity renderableEntity;

    bool operator<(const FilAppRenderable& rhs) const
    {
        return renderableEntity < rhs.renderableEntity;
    }
    bool operator>(const FilAppRenderable& rhs) const { return rhs < *this; }
    bool operator<=(const FilAppRenderable& rhs) const
    {
        return !(rhs < *this);
    }
    bool operator>=(const FilAppRenderable& rhs) const
    {
        return !(*this < rhs);
    }

    void destroy() const
    {
        engine->destroy(renderableEntity);
        engine->destroy(vb);
        engine->destroy(ib);
        // FilAppRenderable does not own material and material instance
    }
};

} // namespace FilApp

#endif // FILAPP_FILAPPRENDERABLE_HPP
