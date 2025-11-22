#pragma once

#include "Id.hpp"

namespace Gecko
{
    class Preview
    {
    public:
        explicit Preview();

        virtual ~Preview();

        void update();

        bool is_visible() const
        {
            return rectangle->isVisible();
        }

        void set_position(std::uint32_t left, std::uint32_t top, const Id& id);
        void set_visible(bool visible);

    private:
        Ogre::Rectangle2D* rectangle { nullptr };
        Ogre::SceneNode* scene_node { nullptr };
        Ogre::MaterialPtr renderMaterial;
        Ogre::RenderTexture* renderTexture { nullptr };
        Ogre::Viewport* viewport { nullptr };

        std::uint16_t texture_size { 256 };
        std::uint16_t size { 128 };
    };
}
