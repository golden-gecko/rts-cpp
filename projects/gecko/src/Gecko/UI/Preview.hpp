#pragma once

#include "Gecko/Cameras/ObjectFollowCamera.hpp"
#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Preview :
        public Updatable
    {
    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Preview();

        virtual ~Preview();

    public:
        std::shared_ptr<ObjectFollowCamera> get_camera() const;

    public:
        void set_position(std::uint32_t left, std::uint32_t top, const Id& id);
        void set_visible(bool visible);

    private:
        Ogre::TexturePtr rttTexture{ nullptr };
        Ogre::RenderTexture* renderTexture { nullptr };
        Ogre::Viewport* viewport { nullptr };

        std::uint16_t texture_size { 256 };
        std::uint16_t size { 128 };
    };
}
