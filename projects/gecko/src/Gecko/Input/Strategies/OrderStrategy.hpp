#pragma once

namespace Gecko
{
    class OrderStrategy
    {
    public:
        virtual void execute(const OIS::MouseEvent& arg) = 0;
    };
}

/*
void Input::process_create_order(const OIS::MouseEvent& arg)
{
    auto layer_result = Utils::Raycast::to_layer(arg);

    if (layer_result)
    {
        OrderManager::getSingleton().order_create(
            Id::Empty, Id::Empty,
            UI::getSingleton().get_configuration_name(),
            layer_result->second,
            Game::getSingleton().get_active_player_id()
        );
    }
}

void Input::process_idle_order(const OIS::MouseEvent& arg)
{
    if (id == OIS::MouseButtonID::MB_Left)
    {
        auto result = Utils::Raycast::cast_ray(arg, Object::QueryFlags::QF_OBJECT);
        auto object = ObjectManager::getSingleton().get(result.first);

        if (object)
        {
            Game::getSingleton().get_active_player()->select(object->get_id(), is_key_pressed(Command::Value::Multiple_Select));
        }
        else
        {
            Game::getSingleton().get_active_player()->select(0, is_key_pressed(Command::Value::Multiple_Select));
        }
    }
    else if (id == OIS::MouseButtonID::MB_Right)
    {
        if (Game::getSingleton().get_active_player()->get_selected()->size())
        {
            for (const auto& selected_object_id : Game::getSingleton().get_active_player()->get_selected())
            {
                auto selected_object = ObjectManager::getSingleton().get(selected_object_id);

                if (selected_object)
                {
                    if (selected_object->get_default_order() == order_type::Value::Move)
                    {
                        auto result = Utils::Raycast::cast_ray(arg, Object::QueryFlags::QF_LAYER);

                        if (is_key_pressed(Command::Value::Multiple_Order) == false)
                        {
                            OrderManager::getSingleton().stop(selected_object_id);
                        }

                        OrderManager::getSingleton().move(selected_object_id, result.second);
                    }
                    else if (selected_object->get_default_order() == order_type::Value::Rally)
                    {
                        auto result = Utils::Raycast::cast_ray(arg, Object::QueryFlags::QF_LAYER);

                        OrderManager::getSingleton().rally(selected_object_id, result.second);
                    }
                }
            }
        }
    }
}

void Input::process_skill(const OIS::MouseEvent& arg, const std::string& skill_name)
{
    if (Game::getSingleton().get_active_player()->get_selected()->size() == 0)
    {
        return;
    }

    auto object_result = Utils::Raycast::to_object(Game::getSingleton().get_map_scene(), arg);
    auto layer_result = Utils::Raycast::to_layer(Game::getSingleton().get_map_scene(), arg);

    for (const auto& object_id : *(Game::getSingleton().get_active_player()->get_selected()))
    {
        auto object = ObjectManager::getSingleton().get(object_id);

        if (object == nullptr)
        {
            continue;
        }

        if (object_result && object_result->first.is_valid())
        {
            object->get_skills()->activate(skill_name, object_result->first);
        }
        else if (layer_result)
        {
            object->get_skills()->activate(skill_name, layer_result->second);
        }
    }
}
*/
