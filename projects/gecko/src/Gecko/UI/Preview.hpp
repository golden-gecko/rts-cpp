#pragma once

#include "Gecko/Cameras/ObjectFollowCamera.hpp"
#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Preview
    {
    public:
        explicit Preview();

        virtual ~Preview();

    public:
        std::shared_ptr<ObjectFollowCamera> get_camera() const;

    public:
        void set_position(std::uint32_t left, std::uint32_t top, const Id& id);
        void set_visible(bool visible);

    private:
        Ogre::TexturePtr     m_texture        = nullptr;
        Ogre::RenderTexture* m_render_texture = nullptr;
        Ogre::Viewport*      m_viewport       = nullptr;

        std::string   m_texture_name = "texture_preview";
        std::uint16_t m_texture_size = 256;
    };
}
