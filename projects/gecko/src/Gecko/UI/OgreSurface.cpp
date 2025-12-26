#include "Gecko/UI/OgreSurface.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Utils/Texture.hpp"

namespace Gecko
{
    OgreSurface::OgreSurface(const std::string& name, std::size_t width, std::size_t height)
    {
        auto size = Ogre::Bitwise::firstPO2From(std::max(width, height));

        // Create texture.
        texture = Utils::Texture::create(name, size, Ogre::PixelFormat::PF_B8G8R8A8);
        pixel_buffer = texture->getBuffer();

        // Create material.
        material = Ogre::MaterialManager::getSingleton().create(
            name,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
        );

        auto pass = material->getTechnique(0)->getPass(0);
        pass->setAmbient(Ogre::ColourValue(1.0f, 1.0f, 1.0f, 0.5f));
        pass->setDepthWriteEnabled(false);
        pass->setLightingEnabled(false);
        pass->setSceneBlending(Ogre::SceneBlendType::SBT_TRANSPARENT_ALPHA);

        auto baseTexUnit = pass->createTextureUnitState(name);
        baseTexUnit->setTextureFiltering(
            Ogre::FilterOptions::FO_ANISOTROPIC,
            Ogre::FilterOptions::FO_ANISOTROPIC,
            Ogre::FilterOptions::FO_NONE);
        baseTexUnit->setTextureAnisotropy(4);

        auto height_to_size = static_cast<float>(height) / static_cast<float>(size);
        auto width_to_size = static_cast<float>(width) / static_cast<float>(size);

        // Create rectangle.
        manual_object = Game::getSingleton().create_manual_object();
        manual_object->begin(name);

        // Triangle #1.
        manual_object->position(-1.0f, 1.0f, 0.0f);
        manual_object->textureCoord(0.0f, 0.0f);
        manual_object->normal(Ogre::Vector3::UNIT_Y);

        manual_object->position(-1.0f, -1.0f, 0.0f);
        manual_object->textureCoord(0.0f, height_to_size);
        manual_object->normal(Ogre::Vector3::UNIT_Y);

        manual_object->position(1.0f, 1.0f, 0.0f);
        manual_object->textureCoord(width_to_size, 0);
        manual_object->normal(Ogre::Vector3::UNIT_Y);

        // Triangle #2.
        manual_object->position(1.0f, 1.0f, 0.0f);
        manual_object->textureCoord(width_to_size, 0);
        manual_object->normal(Ogre::Vector3::UNIT_Y);

        manual_object->position(-1.0f, -1.0f, 0.0f);
        manual_object->textureCoord(0.0f, height_to_size);
        manual_object->normal(Ogre::Vector3::UNIT_Y);

        manual_object->position(1.0f, -1.0f, 0.0f);
        manual_object->textureCoord(width_to_size, height_to_size);
        manual_object->normal(Ogre::Vector3::UNIT_Y);

        manual_object->end();
        manual_object->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
        manual_object->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_OVERLAY);
        manual_object->setUseIdentityProjection(true);
        manual_object->setUseIdentityView(true);

        // Create scene node.
        scene_node = Game::getSingleton().create_scene_node();
        scene_node->attachObject(manual_object);
    }

    OgreSurface::~OgreSurface()
    {
        Game::getSingleton().destroy_manual_object(manual_object);
        Game::getSingleton().destroy_scene_node(scene_node);

        material = nullptr;
        texture = nullptr;
    }

    void OgreSurface::update(ultralight::RefPtr<ultralight::Bitmap> bitmap)
    {
        bitmap->LockPixels();
        pixel_buffer->lock(Ogre::HardwareBuffer::LockOptions::HBL_DISCARD);

        auto pixel_box = pixel_buffer->getCurrentLock();
        auto texture_pitch = pixel_box.rowPitch * Ogre::PixelUtil::getNumElemBytes(pixel_box.format);
        auto destination_buffer = static_cast<Ogre::uint8*>(pixel_box.data);

        auto src_buffer = (std::uint8_t*)bitmap->raw_pixels();
        auto row_bytes = bitmap->row_bytes();

        std::size_t bytes_to_copy = row_bytes > texture_pitch ? texture_pitch : row_bytes;

        for (int y = 0; y < bitmap->height(); ++y)
        {
            std::size_t dst_offset = texture_pitch * y;
            std::size_t src_offset = row_bytes * y;

            std::memcpy(destination_buffer + dst_offset, src_buffer + src_offset, bytes_to_copy);
        }

        pixel_buffer->unlock();
        bitmap->UnlockPixels();
    }
}
