#include "Gecko/DataLayers/DataLayer.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Utils/Texture.hpp"

namespace Gecko
{
    void DataLayer::init()
    {
        auto size = m_owner->get_size();

        m_data.resize(size, std::vector<int>(size));

        if (m_source.empty())
        {
            m_texture = Utils::Texture::create(m_name, size);
        }
        else
        {
            m_texture = Ogre::TextureManager::getSingleton().load(m_source, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

            if (m_texture.isNull())
            {
                throw Exception("Failed to load '" + m_source + "' texture.");
            }

            // TODO: Implement.
            /*
            // Load texture.

            // TODO: Read data from texture.
            // Load data.
            auto image = Utils::Texture::load_square_image(m_source);

            // TODO: Remove. Refactor.
            if (source != owner.get_heightmap())
            {
                // TODO: Make scale and grid scale a scalar.
                if (image.getHeight() != owner.get_size() * (owner.get_scale().x / owner.get_grid_scale().x))
                {
                    throw Exception("File " + source + " size does not match layer parameters (size, scale and grid scale).");
                }
            }

            auto size = static_cast<std::size_t>(image.getHeight());

            auto size = m_owner->get_size();

            m_data.resize(size, std::vector<std::uint8_t>(size));

            std::size_t row_span = image.getRowSpan();
            std::size_t bytes_per_pixel = row_span / size;

            for (std::size_t z = 0; z < size; ++z)
            {
                for (std::size_t x = 0; x < size; ++x)
                {
                    m_data[x][z] = std::array<std::uint8_t, 4>
                    {
                    };
                }
            }
            */
        }
    }

    void DataLayer::deinit()
    {
        m_texture = nullptr;
    }

    void DataLayer::update(float time)
    {
        if (m_dirty)
        {
            const auto& pixel_buffer = m_texture->getBuffer();
            pixel_buffer->lock(Ogre::HardwareBuffer::LockOptions::HBL_WRITE_ONLY);

            const auto& pixel_box = pixel_buffer->getCurrentLock();
            auto texture_depth = Ogre::PixelUtil::getNumElemBytes(pixel_box.format);
            auto texture_pitch = pixel_box.rowPitch * texture_depth;
            auto destination_buffer = static_cast<std::uint8_t*>(pixel_box.data);

            for (int x = 0; x < m_data.size(); ++x)
            {
                for (int z = 0; z < m_data[x].size(); ++z)
                {
                    std::uint8_t color[4] = { 0 };

                    if (m_data[x][z] > 0)
                    {
                        color[0] = 0xFF;
                        color[3] = 0xFF;
                    }

                    std::memcpy(destination_buffer + (z * texture_pitch + (x * texture_depth)), color, 4);
                }
            }

            pixel_buffer->unlock();

            m_dirty = false;
        }
    }

    DataLayer::DataLayer(Layer* owner, const std::string& name, const Configuration& configuration) :
        m_owner(owner), m_name(name)
    {
        m_source = configuration.get_string("source", "");
    }

    int DataLayer::get_data(int x, int z) const
    {
        if (x < m_data.size())
        {
            if (z < m_data[x].size())
            {
                return m_data[x][z];
            }
        }

        L_WARNING << "DataLayer::get_data(" << x << ", " << z << "): Index outside range.";

        return 0;
    }

    void DataLayer::add(const Navigation::Coordinate& index, const Area& area, int value)
    {
        for (int z = index.z - area.get_start().z; z <= index.z + area.get_end().z; ++z)
        {
            for (int x = index.x - area.get_start().x; x <= index.x + area.get_end().x; ++x)
            {
                if (m_owner->is_index_valid(x, z))
                {
                    m_data[x][z] = std::clamp(m_data[x][z] + value, 0, 255);
                }
            }
        }

        m_dirty = true;
    }

    void DataLayer::remove(const Navigation::Coordinate& index, const Area& area, int value)
    {
        for (int z = index.z - area.get_start().z; z <= index.z + area.get_end().z; ++z)
        {
            for (int x = index.x - area.get_start().x; x <= index.x + area.get_end().x; ++x)
            {
                if (m_owner->is_index_valid(x, z))
                {
                    m_data[x][z] = std::clamp(m_data[x][z] - value, 0, 255);
                }
            }
        }

        m_dirty = true;
    }
}
