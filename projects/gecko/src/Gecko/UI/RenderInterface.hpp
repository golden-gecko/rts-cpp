#pragma once

namespace Gecko
{
    struct RocketCompiledGeometry
    {
        Ogre::RenderOperation mRenderOperation;
        Rml::Texture* mTexture = nullptr;
        std::string           mTextureName;
    };

    struct RocketVertex
    {
        Ogre::Real x, y, z;
        Ogre::uint32 diffuse;
        Ogre::Real u, v;
    };

    class RenderInterface :
        public Rml::RenderInterface
    {
    public:
        // From Rml::RenderInterface.
        Rml::CompiledGeometryHandle CompileGeometry(Rml::Span<const Rml::Vertex> vertices, Rml::Span<const int> indices) override;
        void RenderGeometry(Rml::CompiledGeometryHandle geometry, Rml::Vector2f translation, Rml::TextureHandle texture) override;
        void ReleaseGeometry(Rml::CompiledGeometryHandle geometry) override;
        Rml::TextureHandle LoadTexture(Rml::Vector2i& texture_dimensions, const Rml::String& source) override;
        Rml::TextureHandle GenerateTexture(Rml::Span<const Rml::byte> source, Rml::Vector2i source_dimensions) override;
        void ReleaseTexture(Rml::TextureHandle texture) override;
        void EnableScissorRegion(bool enable) override;
        void SetScissorRegion(Rml::Rectanglei region) override;

    public:
        RenderInterface(unsigned int window_width, unsigned int window_height);

    private:
        Ogre::RenderSystem* m_render_system = nullptr;
        unsigned int        m_window_width = 0;
        unsigned int        m_window_height = 0;
        bool                m_scissor_enable = false;
        size_t              m_scissor_rect[4] = { 0, 0, 0, 0 };
        Ogre::String        m_group;
    };
}
