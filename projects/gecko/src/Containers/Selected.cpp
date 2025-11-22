#include "Containers/Selected.hpp"

#include "Containers/Configurations.hpp"
#include "Containers/Orders.hpp"
#include "Containers/Skills.hpp"
#include "Games/Game.hpp"
#include "Managers/ConfigurationManager.hpp"
#include "Log.hpp"
#include "Managers/ObjectManager.hpp"
#include "Objects/Object.hpp"
#include "Skills/Skill.hpp"
#include "UI/UI.hpp"
#include "Utils/Convert.hpp"

namespace Gecko
{
    std::shared_ptr<Configuration> Selected::serialize() const
    {
        auto configuration = std::make_shared<Configuration>();

        for (const auto& i : m_items)
        {
            configuration->append("items", i);
        }

        for (const auto& [number, group] : groups)
        {
            for (const auto& i : group)
            {
                configuration->append("groups." + Utils::Convert::to_string(i), i);
            }
        }

        return configuration;
    }

    void Selected::deserialize(const std::shared_ptr<Configuration>& configuration)
    {
        clear();

        // TODO: Implement.
    }


    void Selected::apply_current_selection(bool select)
    {
        apply_selection(m_items, select);
        apply_ui(select);
    }

    void Selected::apply_ui(bool select)
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
        else
        {
            for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
            {
                if (configuration->get_bool("creatable.by_player", false))
                {
                    configurations.emplace(name);
                }
            }
        }

        auto& ui = UI::getSingleton();

        ui.set_configurations(configurations);
        ui.set_orders(orders);
        ui.set_skills(skills);
    }

    void Selected::create_group(std::uint16_t group_number)
    {
        UI::getSingleton().log_info(
            "Assigned "
            + Utils::Convert::to_string(m_items.size())
            + " objects to "
            + Utils::Convert::to_string(group_number) + " group"
        );

        groups[group_number] = m_items;
    }

    void Selected::select(Id object_id, bool add)
    {
        L_TRACE << "Player::select(" << object_id << ", " << add << ")";

        Items objects_ids =
        {
            object_id
        };

        select(objects_ids, add);
    }

    void Selected::select(const Items& object_ids, bool add)
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

    void Selected::select_group(std::uint16_t group_number)
    {
        UI::getSingleton().log_info(
            "Selected "
            + Utils::Convert::to_string(groups[group_number].size())
            + " objects from "
            + Utils::Convert::to_string(group_number) + " group"
        );

        apply_selection(m_items, false);

        select(groups[group_number]);
    }

    void Selected::apply_selection(const Items& object_ids, bool select)
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
