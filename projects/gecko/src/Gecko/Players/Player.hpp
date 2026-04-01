#pragma once

#include "Gecko/Item.hpp"

namespace Gecko
{
    class Player :
        public Item
    {
    private:
        using base_type = Item;

    // From Serializable.
    public:
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    // From Updatable.
    public:
        void update(float time) override;

    // Player.
    public:
        static PlayerPtr create(PlayerPtr memory, const ConfigurationPtr& configuration);

    public:
        Player();

    public:
        Player& operator=(const Player&) = delete;

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
