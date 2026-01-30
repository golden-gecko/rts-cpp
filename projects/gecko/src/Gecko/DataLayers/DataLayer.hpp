#pragma once

#include "Gecko/Area.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Navigation/Coordinate.hpp"

namespace Gecko
{
    class DataLayer :
        public Initializable,
        public Updatable
    {
    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit DataLayer(Layer* owner, const std::string& name, const Configuration& configuration);

    public:
        const auto get_data(int x, int z) const
        {
            // TODO: Check range.
            return m_data[x][z];
        }

        const auto& get_name() const
        {
            return m_name;
        }

        Ogre::TexturePtr get_texture() const
        {
            return m_texture;
        }

    public:
        void add(const Navigation::Coordinate& index, const Area& area, int value);
        void remove(const Navigation::Coordinate& index, const Area& area, int value);

    private:
        Layer* m_owner = nullptr;

        std::string m_name;
        std::string m_source;

        std::vector<std::vector<int>> m_data;

        Ogre::TexturePtr m_texture;
        bool m_dirty = false;
    };
}
