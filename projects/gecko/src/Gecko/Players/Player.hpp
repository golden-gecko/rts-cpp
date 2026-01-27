#pragma once

#include "Gecko/Item.hpp"
#include "Gecko/Containers/Selected.hpp"

namespace Gecko
{
    class Player :
        public Item
    {
    private:
        using base_type = Item;

    public:
        static Player* create();
        static Player* create(const ConfigurationPtr& configuration);
        static Player* create(Player* memory);
        static Player* create(Player* memory, const ConfigurationPtr& configuration);

    public:
        explicit Player();
        explicit Player(const Player& other);

        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

        void update(float time) override;

    public:
        const auto& get_color() const
        {
            return m_color;
        }

        auto get_diplomacy()
        {
            return m_diplomacy;
        }

        const auto& get_name() const
        {
            return m_name;
        }

        auto get_resources()
        {
            return m_resources;
        }

        auto get_selected()
        {
            return m_selected;
        }

        Id get_first_selected()
        {
            if (m_selected->size() > 0)
            {
                return *(m_selected->begin());
            }

            return Id::Empty;
        }

    private:
        std::string m_name;
        std::string m_color;

        std::shared_ptr<Diplomacy> m_diplomacy;
        std::shared_ptr<Resources> m_resources;
        std::shared_ptr<Selected>  m_selected;
    };
}
