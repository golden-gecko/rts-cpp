#pragma once

namespace Gecko
{
    class OgreSurface
    {
    public:
        OgreSurface(const std::string& name, std::size_t width, std::size_t height);

        virtual ~OgreSurface();

        void update(ultralight::RefPtr<ultralight::Bitmap> bitmap);

        bool is_visible() const
        {
            return manual_object->isVisible();
        }

        void set_visible(bool visible)
        {
            scene_node->setVisible(visible);
        }

    private:
        Ogre::TexturePtr texture;
        Ogre::HardwarePixelBufferSharedPtr pixel_buffer;

        Ogre::MaterialPtr material;

        Ogre::ManualObject* manual_object = nullptr;
        Ogre::SceneNode* scene_node = nullptr;
    };
}
