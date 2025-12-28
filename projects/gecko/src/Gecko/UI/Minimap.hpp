#pragma once

namespace Gecko
{
    class Minimap
    {
    public:
        explicit Minimap();

        virtual ~Minimap();

        void update();

        void click(int x, int y);
        void move(int x, int y);
        void zoom_in();
        void zoom_out();

        bool is_visible() const
        {
            return rectangle->isVisible();
        }

        void set_visible(bool visible)
        {
            scene_node->setVisible(visible);
        }

    private:
        Ogre::Rectangle2D* rectangle { nullptr };
        Ogre::SceneNode* scene_node { nullptr };
        Ogre::MaterialPtr renderMaterial;
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
