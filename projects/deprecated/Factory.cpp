if (get_initialized() && is_enabled())
{
    if (orders.size())
    {
        auto order = orders.front();

        if (order)
        {
            switch (order->get_type())
            {
                case order_type::Value::Create:
                {
                    auto create_order = dynamic_cast<OrderCreate*>(order);

                    if (create_order)
                    {
                        // Check if player have enough resources to build object.
                        auto player = PlayerManager::getSingleton().get(get_player_id());
                        auto configuration = ConfigurationManager::getSingleton().get(create_order->get_configuration_name());
                        auto _costs = configuration->get_costs();

                        if (player)
                        {
                            if (player->has_resources(_costs))
                            {
                                // Create object.
                                auto object = ObjectManager::getSingleton().create(create_order->get_configuration_name());

                                if (object)
                                {
                                    object->set_player_id(get_player_id());

                                    if (object->is_order_possible(order_type::Value::Move))
                                    {
                                        object->set_position(get_position());
                                        OrderManager::getSingleton().move(object->get_id(), rally_point);
                                    }
                                    else
                                    {
                                        object->set_position(rally_point);
                                    }

                                    player->remove_resources(_costs);

                                    float current_spent_resources = 0.0f;

                                    for (const auto& i : _costs)
                                    {
                                        current_spent_resources += i.second;
                                    }

                                    spent_resources += current_spent_resources;
                                    Statistics::getSingleton().add("Spent resources", current_spent_resources);
                                    // map->get_terrain().add_pollution(get_position(), current_spent_resources * 0.1f);

                                    ++created_objects;
                                }

                                remove_current_order();
                            }
                            else
                            {
                                L_WARNING << "Not enough resources to build " << create_order->get_configuration_name() << ".";
                            }
                        }
                    }

                    break;
                }

                case order_type::Value::Destroy:
                    on_destroy();
                    break;

                case order_type::Value::Rally:
                {
                    auto rally_order = dynamic_cast<OrderRally*>(order);

                    if (rally_order)
                    {
                        rally_point = rally_order->get_target_position();

                        delete orders.front();
                        orders.pop_front();
                    }

                    break;
                }

                default:
                    remove_current_order();
            }
        }
    }
}

        /*
        if (is_enabled())
        {
            // TODO: Move to configuration.
            if (get_time_alive() >= 10.0f)
            {
                ObjectManager::getSingleton().destroy(get_id());
            }
            else
            {
                set_position(get_position() + get_direction() * speed * time);
            }
        }
        */

               /*
        if (map->get_terrain().is_valid(get_position()))
        {
            scene_node->setPosition(position);
        }
        else
        {
            OrderManager::getSingleton().destroy(get_id());
        }
        */