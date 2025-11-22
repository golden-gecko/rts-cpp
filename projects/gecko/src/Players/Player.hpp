#pragma once

#include "Item.hpp"

namespace Gecko
{
    class Player : public Item
    {
    private:
        using base_type = Item;

    public:
        static Player* create();
        static Player* create(const std::shared_ptr<Configuration>& configuration);
        static Player* create(Player* memory);
        static Player* create(Player* memory, const std::shared_ptr<Configuration>& configuration);

    public:
        explicit Player();
        explicit Player(const Player& other);

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

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

    private:
        std::string m_name;
        std::string m_color;

        std::shared_ptr<Diplomacy> m_diplomacy;
        std::shared_ptr<Resources> m_resources;
        std::shared_ptr<Selected> m_selected;
    };
}
