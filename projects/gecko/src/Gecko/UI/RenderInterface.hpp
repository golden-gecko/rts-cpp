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
        Ogre::RenderSystem*    mRenderSystem = nullptr;
        Ogre::LayerBlendModeEx mColourBlendMode;
        Ogre::LayerBlendModeEx mAlphaBlendMode;
        bool                   mScissorEnable = false;
        size_t                 mScissorRect[4] = { 0, 0, 0, 0 };
        Ogre::String           mGroup;
    };
}
