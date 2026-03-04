#include "Gecko/Containers/Selection.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Configurations.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Skills.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/Skills/Skill.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/UI/Widgets/Configurations.hpp"
#include "Gecko/UI/Widgets/Orders.hpp"
#include "Gecko/UI/Widgets/Skills.hpp"
#include "Gecko/Utils/Convert.hpp"

namespace Gecko
{
    ConfigurationPtr Selection::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : m_items)
        {
            configuration->append("items", i);
        }

        for (const auto& [number, group] : m_groups)
        {
            for (const auto& i : group)
            {
                configuration->append("groups." + Utils::Convert::to_string(i), i);
            }
        }

        return configuration;
    }

    void Selection::deserialize(const ConfigurationPtr& configuration)
    {
        clear();

        // TODO: Implement.
    }


    void Selection::apply_current_selection(bool select)
    {
        apply_selection(m_items, select);
        apply_ui(select);
    }

    void Selection::apply_ui(bool select)
    {
        std::set<std::string> configurations;
        std::set<std::string> orders;
        std::set<std::string> skills;

        if (select && m_items.empty() == false)
        {
            for (const auto& object_id : m_items)
            {
                auto object = ObjectManager::getSingleton().get(object_id);

                if (object)
                {
                    for (const auto& i : *(object->get_configurations()))
                    {
                        configurations.emplace(i);
                    }

                    for (const auto& i : *(object->get_orders()))
                    {
                        orders.emplace(order_type::to_string(i));
                    }

                    for (const auto& i : *(object->get_skills()))
                    {
                        skills.emplace(i->get_configuration()->get_name());
                    }
                }
            }
        }

        {
            auto configurations_widget = UI::getSingleton().get_component<ConfigurationsWidget>();
            auto orders_widget = UI::getSingleton().get_component<OrdersWidget>();
            auto skills_widget = UI::getSingleton().get_component<SkillsWidget>();

            if (configurations_widget)
            {
                configurations_widget->update(configurations);
            }

            if (orders_widget)
            {
                orders_widget->update(orders);
            }

            if (skills_widget)
            {
                skills_widget->update(skills);
            }
        }
    }

    void Selection::create_group(std::uint16_t group_number)
    {
        UI::getSingleton().log_info(
            "Assigned "
            + Utils::Convert::to_string(m_items.size())
            + " objects to "
            + Utils::Convert::to_string(group_number) + " group"
        );

        m_groups[group_number] = m_items;
    }

    void Selection::select(const Id& object_id, bool add)
    {
        L_TRACE << "Player::select(" << object_id << ", " << add << ")";

        Items objects_ids =
        {
            object_id
        };

        select(objects_ids, add);
    }

    void Selection::select(const Items& object_ids, bool add)
    {
        L_TRACE << "Player::select(" << object_ids.size() << ", " << add << ")";

        if (add == false)
        {
            clear();
        }

        for (const auto& id : object_ids)
        {
            auto object = ObjectManager::getSingleton().get(id);

            if (object == nullptr)
            {
                L_WARNING << "Cannot select object " << id << " " << ", because it does not exist.";

                continue;
            }

            if (object->is_selectable() == false)
            {
                L_WARNING << "Cannot select object " << object->get_id() << " " << object->get_name() << ", because it is not selectable.";

                continue;
            }

            if (object->get_player_id() != Game::getSingleton().get_active_player_id())
            {
                L_WARNING << "Cannot select object from non-active player (" << object->get_player_id() << ", " << Game::getSingleton().get_active_player_id() << ").";

                continue;
            }

            m_items.emplace(id);
        }

        apply_current_selection(true);
    }

    void Selection::select_group(std::uint16_t group_number)
    {
        UI::getSingleton().log_info(
            "Selection "
            + Utils::Convert::to_string(m_groups[group_number].size())
            + " objects from "
            + Utils::Convert::to_string(group_number) + " group"
        );

        apply_selection(m_items, false);

        select(m_groups[group_number]);
    }

    void Selection::apply_selection(const Items& object_ids, bool select)
    {
        for (const auto& id : object_ids)
        {
            auto object = ObjectManager::getSingleton().get(id);

            if (object)
            {
                object->set_selected(select);
            }
        }
    }
}
