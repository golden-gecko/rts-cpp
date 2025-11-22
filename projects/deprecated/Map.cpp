void Map::load_terrain(const std::shared_ptr<Configuration>& configuration)
{
    auto terrain_configuration = configuration->get_child_optional("terrain");

    if (terrain_configuration)
    {
        terrain = std::make_unique<Terrain>(terrain_configuration.get());
        terrain->init();
        terrain->set_layer(LayerType::to_string(LayerType::Value::None));
        terrain->set_search_strategy(Search::Type::AvoidOccupied);

        std::set<std::string> layers;

        auto layers_configuration = terrain_configuration.get().get_child("layers");

        for (auto i = layers_configuration.begin(); i != layers_configuration.end(); ++i)
        {
            layers.emplace(i->get("type", "").asString());
        }

        UI::getSingleton().set_terrain_layers(layers);
    }
}

void Map::load_water(const std::shared_ptr<Configuration>& configuration)
{
    auto water_configuration = configuration->get_child_optional("water");

    if (water_configuration)
    {
        water = std::make_unique<Water>(water_configuration.get());
        water->init();
        water->set_layer(LayerType::to_string(LayerType::Value::None));
        water->set_search_strategy(Search::Type::AvoidOccupied);

        std::set<std::string> layers;

        auto layers_configuration = water_configuration.get().get_child("layers");

        for (auto i = layers_configuration.begin(); i != layers_configuration.end(); ++i)
        {
            layers.emplace(i->get("type", "").asString());
        }

        UI::getSingleton().set_water_layers(layers);
    }
}

void Map::set_fog_of_war_texture(const Ogre::TexturePtr& texture)
{
    boost::ignore_unused(texture);

    // TODO: Add option to disable fog of war.
    // get_terrain().set_fog_of_war_texture(texture);
    // get_water().set_fog_of_war_texture(texture);
}

void Map::show_grid(bool visible)
{
    /*
    TODO: Delete.
    if (terrain)
    {
        terrain->show_grid(visible);
    }

    if (water)
    {
        water->show_grid(visible);
    }
    */
}

bool Map::is_grid_visible() const
{
    /*
    TODO: Delete.
    if (terrain)
    {
        return terrain->is_grid_visible() && water->is_grid_visible();
    }
    */

    return false;
}

void show_grid(bool visible);

bool is_grid_visible() const;

void set_fog_of_war_texture(const Ogre::TexturePtr& texture);

// TODO: Fix and test.
void Map::init_random_objects()
{
    auto random_objects_configuration = configuration->get_child_optional("random_objects");

    if (random_objects_configuration)
    {
        for (const auto& i : *(*(random_objects_configuration)))
        {
            auto random_objects_set = Configuration(i);

            auto min = Ogre::Vector3(random_objects_set.get_float("area.min.x"), 0.0f, random_objects_set.get_float("area.min.z"));
            auto max = Ogre::Vector3(random_objects_set.get_float("area.max.x"), 0.0f, random_objects_set.get_float("area.max.z"));

            for (const auto& j : *(random_objects_set.get_child("objects")))
            {
                auto objects_set = Configuration(j);

                auto configuration = objects_set.get_string("configuration");
                auto player_name = objects_set.get_string("player");
                auto count = objects_set.get_int("count");

                for (auto k = 0; k < count; ++k)
                {
                    auto object = ObjectManager::getSingleton().create(configuration);

                    if (object == nullptr)
                    {
                        throw Exception("Failed to create '" + configuration + "' object.");
                    }

                    object->init();

                    for (const auto& [id, player] : PlayerManager::getSingleton())
                    {
                        if (player->get_configuration()->get_name() == player_name)
                        {
                            object->set_player_id(id);

                            break;
                        }
                    }

                    object->set_position(Random::get_position(min, max));
                }
            }
        }
    }
}
