#include "Gecko/Utils/Texture.hpp"

#include "Gecko/Exception.hpp"
#include "Gecko/Utils/Math.hpp"

namespace Gecko::Utils::Texture
{
    void clear(const Ogre::TexturePtr& texture, std::uint8_t value)
    {
        const Ogre::HardwarePixelBufferPtr& pixel_buffer = texture->getBuffer();

        pixel_buffer->lock(Ogre::HardwareBuffer::LockOptions::HBL_DISCARD);

        const Ogre::PixelBox& pixel_box = pixel_buffer->getCurrentLock();
        Ogre::uint8 texture_depth = Ogre::PixelUtil::getNumElemBytes(pixel_box.format);
        std::size_t texture_pitch = pixel_box.rowPitch * texture_depth;
        Ogre::uint8* destination_buffer = static_cast<Ogre::uint8*>(pixel_box.data);

        std::memset(destination_buffer, value, texture_pitch * texture->getHeight());

        pixel_buffer->unlock();
    }

    Ogre::TexturePtr create(const std::string& name, int size, Ogre::PixelFormat format, Ogre::TextureUsage usage)
    {
        auto texture = Ogre::TextureManager::getSingleton().createManual(
            name,
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            Ogre::TextureType::TEX_TYPE_2D,
            size,
            size,
            0,
            format,
            usage
        );

        clear(texture, 0);

        return texture;
    }

    Ogre::TexturePtr get(const std::string& texture_name)
    {
        return Ogre::TextureManager::getSingleton().getByName(texture_name);
    }

    Ogre::Image load_square_image(const std::string& file_name)
    {
        Ogre::Image image;

        image.load(file_name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        if (image.getHeight() != image.getWidth())
        {
            throw Exception("File '" + file_name + "' is not square.");
        }

        if (Math::is_power_2(image.getHeight()) == false)
        {
            throw Exception("File '" + file_name + "' size is not power of 2.");
        }

        if (image.getFormat() != Ogre::PixelFormat::PF_A8B8G8R8)
        {
            throw Exception("File '" + file_name + "' is not in ABGR format.");
        }

        return image;
    }

    void update(const Ogre::TexturePtr& texture, int x, int y, Ogre::uint8 color[])
    {
        const Ogre::HardwarePixelBufferPtr& pixel_buffer = texture->getBuffer();

        pixel_buffer->lock(Ogre::HardwareBuffer::LockOptions::HBL_WRITE_ONLY);

        const Ogre::PixelBox& pixel_box = pixel_buffer->getCurrentLock();
        Ogre::uint8 texture_depth = Ogre::PixelUtil::getNumElemBytes(pixel_box.format);
        std::size_t texture_pitch = pixel_box.rowPitch * texture_depth;
        Ogre::uint8* destination_buffer = static_cast<Ogre::uint8*>(pixel_box.data);

        // TODO: Check image format (order of byte colors).
        destination_buffer[y * texture_pitch + (x * texture_depth)    ] = color[2]; // B
        destination_buffer[y * texture_pitch + (x * texture_depth) + 1] = color[1]; // G
        destination_buffer[y * texture_pitch + (x * texture_depth) + 2] = color[0]; // R
        destination_buffer[y * texture_pitch + (x * texture_depth) + 3] = color[3]; // A

        pixel_buffer->unlock();
    }

    Ogre::TextureUnitState* get_unit_state(const std::string& material_name, const std::string& texture_unit_state_name)
    {
        Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(material_name);

        if (material.get() == nullptr)
        {
            throw Exception("Material '" + material_name + "' not found.");
        }

        for (Ogre::Technique* technique : material->getTechniques())
        {
            for (Ogre::Pass* pass : technique->getPasses())
            {
                if (Ogre::TextureUnitState* texture_unit_state = pass->getTextureUnitState(texture_unit_state_name))
                {
                    return texture_unit_state;
                }
            }
        }

        throw Exception("Texture unit state '" + texture_unit_state_name + "' not found.");
    }
}
