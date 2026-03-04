#pragma once

#include "Gecko/Item.hpp"

namespace Gecko
{
    class Player :
        public Item
    {
    private:
        using base_type = Item;

    public:
        static const std::string Name;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        static PlayerPtr create(PlayerPtr memory, const ConfigurationPtr& configuration);

    public:
        explicit Player();
        explicit Player(const Player& other);

    public:
        const std::string& get_name() const
        {
            return m_name;
        }

        const std::string& get_color() const
        {
            return m_color;
        }

        const ResourcesPtr& get_resources() const
        {
            return m_resources;
        }

        const SelectionPtr& get_selected() const
        {
            return m_selected;
        }

        Id get_first_selected();

    private:
        std::string m_name;
        std::string m_color;

        ResourcesPtr m_resources;
        SelectionPtr m_selected;
    };
}
