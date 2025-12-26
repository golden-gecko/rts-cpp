#pragma once

namespace Gecko::Utils::Texture
{
    void clear(const Ogre::TexturePtr& texture, std::uint8_t value = 0);

    Ogre::TexturePtr create(
        const std::string& name,
        int size,
        Ogre::PixelFormat format = Ogre::PixelFormat::PF_R8G8B8A8,
        Ogre::TextureUsage usage = Ogre::TextureUsage::TU_DEFAULT
    );

    Ogre::TexturePtr get(const std::string& texture_name);

    Ogre::Image load_square_image(const std::string& file_name);

    void update(const Ogre::TexturePtr& texture, int x, int y, Ogre::uint8 color[]);

    Ogre::TextureUnitState* get_unit_state(const std::string& material_name, const std::string& texture_unit_state_name);
}
