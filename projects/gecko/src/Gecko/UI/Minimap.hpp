#pragma once

#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Minimap :
        public Updatable
    {
    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Minimap();

        virtual ~Minimap();

    public:
        void click(int x, int y);
        void move(int x, int y);
        void zoom_in();
        void zoom_out();

    private:
        Ogre::TexturePtr rttTexture{ nullptr };
        Ogre::RenderTexture* renderTexture { nullptr };
        Ogre::Viewport* viewport { nullptr };

        std::uint16_t texture_size { 256 };
        std::uint16_t size { 128 };

        float min_zoom { 1.0f };
        float max_zoom { 16.0f };

        float scroll_sensivity { 0.005f };
        float zoom_sensivity { 2.0f };
    };
}
