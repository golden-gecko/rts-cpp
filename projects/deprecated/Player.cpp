std::map<std::string, Diplomacy::State> diplomacy;

bool is(Id player_id, Diplomacy::State state) const
{
	auto diplomacy_state = diplomacy.find(player_id);

	if (diplomacy_state == diplomacy.end())
	{
	return false;
	}

	return diplomacy_state->second == state;

	return false;
}

void select(Id object_id, bool multiple);
void apply_selection(const std::vector<Id>& objects, bool select);
void assign_selected_to_group(int group_number);
void select_group(int group_number);

void remove_from_selection(Id object_id)
{
	selected.erase(std::remove(selected.begin(), selected.end(), object_id), selected.end());

	for (auto& group : groups)
	{
		group.second.erase(std::remove(group.second.begin(), group.second.end(), object_id), group.second.end());
	}
}

void apply_fog_of_war();
void set_unit_position(const Id& id, const Ogre::Vector3& position, float visibility_range);
void update_unit_position(const Id& id, const Ogre::Vector3& old, const Ogre::Vector3& current, float visibility_range);
void remove_unit_from_position(const Id& id, const Ogre::Vector3& position, float visibility_range);

Ogre::uint8 fog_of_war_reveal_color[4] { 0, 0, 0, 0 };
Ogre::uint8 fog_of_war_hide_color[4] { 0, 0, 0, 0 };
Ogre::TexturePtr fog_of_war_texture;

struct Node
{
	std::set<Id> objects;
	std::uint32_t visibility_count { 0 };
};

std::vector<std::vector<Node>> unit_position_layer;

void update_texture(const Ogre::TexturePtr& texture, const Ogre::Vector3& position, float visibility_range, bool reveal);

Player::Player(const std::string& configuration_name) :
    base_type(configuration_name)
{
    auto configuration = ConfigurationManager::getSingleton().get(configuration_name);

    // TODO: Virtual method should not be called from constructor.
    deserialize(configuration);

    for (const auto& [name, status] : configuration->get_child("diplomacy"))
    {
        diplomacy.emplace(std::make_pair(name, Diplomacy::from_name(status.data())));
    }

    fog_of_war_reveal_color[0] = 255;
    fog_of_war_reveal_color[1] = 255;
    fog_of_war_reveal_color[2] = 255;
    fog_of_war_reveal_color[3] = 255;

    {
        auto configuration = ConfigurationManager::getSingleton().get(
            ConfigurationType::Game, 
            Game::getSingleton().get_configuration_name()
        );

        TODO: Fix.
        fog_of_war_hide_color[0] = configuration.get<Ogre::uint8>("options.map.fog_of_war.color.r");
        fog_of_war_hide_color[1] = configuration.get<Ogre::uint8>("options.map.fog_of_war.color.g");
        fog_of_war_hide_color[2] = configuration.get<Ogre::uint8>("options.map.fog_of_war.color.b");
        fog_of_war_hide_color[3] = configuration.get<Ogre::uint8>("options.map.fog_of_war.color.a");

        auto size = 32;

        // TODO: Fix naming.
        fog_of_war_texture = Utils::Texture::create("fog_of_war_" + name, size);

        unit_position_layer.resize(size, std::vector<Node>(size));
    }
}

Player::Player(const Player& other) :
    base_type(other)
{
    name = other.name;
    color = other.color;
    resources = other.resources;
    diplomacy = other.diplomacy;
    selected = other.selected;

    /*
    fog_of_war_reveal_color[0] = other.fog_of_war_reveal_color[0];
    fog_of_war_reveal_color[1] = other.fog_of_war_reveal_color[1];
    fog_of_war_reveal_color[2] = other.fog_of_war_reveal_color[2];
    fog_of_war_reveal_color[3] = other.fog_of_war_reveal_color[3];

    {
        auto configuration = ConfigurationManager::getSingleton().get(
            ConfigurationType::Game, 
            Game::getSingleton().get_configuration_name()
        );

        fog_of_war_hide_color[0] = other.fog_of_war_hide_color[0];
        fog_of_war_hide_color[1] = other.fog_of_war_hide_color[1];
        fog_of_war_hide_color[2] = other.fog_of_war_hide_color[2];
        fog_of_war_hide_color[3] = other.fog_of_war_hide_color[3];

        auto size = 32;

        // TODO: Fix naming.
        // fog_of_war_texture = Utils::Texture::create("fog_of_war_" + name, size);

        unit_position_layer.resize(size, std::vector<Node>(size));
    }
}
*/


/*
    // Display orders.
    Object::PossibleOrdersList orders;

    for (const auto& selected_object_id : selected)
    {
        auto selected_object = ObjectManager::getSingleton().get(selected_object_id);

        if (selected_object)
        {
            for (const auto& order_type : selected_object->get_possible_orders())
            {
                orders.emplace(order_type);
            }
        }
    }

    UI::getSingleton().set_orders(orders);

    // Display configurations.
    auto configurations = std::set<std::string>();

    if (selected.size())
    {
        for (const auto& selected_object_id : selected)
        {
            auto selected_object = ObjectManager::getSingleton().get(selected_object_id);

            if (selected_object)
            {
                auto factory = dynamic_cast<Factory*>(selected_object);

                if (factory)
                {
                    for (const auto& _configuration_name : factory->configurations())
                    {
                        configurations.emplace(_configuration_name);
                    }
                }
            }
        }
    }

    // TODO: Read configurations from object.
    std::set<std::string> configurations;

    for (const auto& [name, configuration] : ConfigurationManager::getSingleton().get_configurations(ConfigurationType::Object))
    {
        if (configuration->get_bool("creatable.by_player", false))
        {
            configurations.emplace(name);
        }
    }

    UI::getSingleton().set_configurations(configurations);

void Player::apply_fog_of_war()
{
    // TODO: Fix.
    /*
    for (Ogre::uint32 x = 0; x < fog_of_war_texture->getWidth(); ++x)
    {
        for (Ogre::uint32 z = 0; z < fog_of_war_texture->getHeight(); ++z)
        {
            const auto& all_object_ids = Game::getSingleton().get_active_map()->get_terrain().occupied_layer.get(x, z).objects;

            if (unit_position_layer[x][z].visibility_count > 0)
            {
                for (const auto& object_id : all_object_ids)
                {
                    auto object = ObjectManager::getSingleton().get(object_id);

                    if (object)
                    {
                        object->set_visible(true);
                    }
                }
            }
            else
            {
                for (const auto& object_id : all_object_ids)
                {
                    auto object = ObjectManager::getSingleton().get(object_id);

                    if (object)
                    {
                        object->set_visible(false);
                    }
                }
            }
        }
    }
}

void Player::set_unit_position(const Id& id, const Ogre::Vector3& position, float visibility_range)
{
    auto index = Game::getSingleton().get_active_map()->get_terrain().get_index_from_position(position);

    unit_position_layer[index.first][index.second].objects.emplace(_id);

    update_texture(fog_of_war_texture, position, visibility_range, true);
}

void Player::update_unit_position(const Id& id, const Ogre::Vector3& old, const Ogre::Vector3& current, float visibility_range)
{
    auto old_index = Game::getSingleton().get_active_map()->get_terrain().get_index_from_position(old);
    auto current_index = Game::getSingleton().get_active_map()->get_terrain().get_index_from_position(current);

    if (old_index != current_index)
    {
        unit_position_layer[old_index.first][old_index.second].objects.erase(_id);
        unit_position_layer[current_index.first][current_index.second].objects.emplace(_id);

        update_texture(fog_of_war_texture, old, visibility_range, false);
        update_texture(fog_of_war_texture, current, visibility_range, true);
    }
}

void Player::remove_unit_from_position(const Id& id, const Ogre::Vector3& position, float visibility_range)
{
    auto index = Game::getSingleton().get_active_map()->get_terrain().get_index_from_position(position);

    unit_position_layer[index.first][index.second].objects.erase(_id);

    update_texture(fog_of_war_texture, position, visibility_range, false);
}

void Player::update_texture(const Ogre::TexturePtr& texture, const Ogre::Vector3& position, float visibility_range, bool reveal)
{
    auto pixel_buffer = texture->getBuffer();

    pixel_buffer->lock(Ogre::HardwareBuffer::LockOptions::HBL_WRITE_ONLY);

    auto pixel_box = pixel_buffer->getCurrentLock();
    auto texture_depth = Ogre::PixelUtil::getNumElemBytes(pixel_box.format);
    auto texture_pitch = pixel_box.rowPitch * texture_depth;
    auto destination_buffer = static_cast<Ogre::uint8*>(pixel_box.data);

    auto index = Game::getSingleton().get_active_map()->get_terrain().get_index_from_position(position);
    auto scale = Game::getSingleton().get_active_map()->get_terrain().get_scale();

    // TODO: Make sure that terrain and water have the scale in X and Z axes.
    int ceiled_radius = static_cast<int>(std::ceilf(visibility_range / scale.x)) - 1;

    int x_start = index.first - ceiled_radius;
    int x_end = index.first + ceiled_radius;

    int z_start = index.second - ceiled_radius;
    int z_end = index.second + ceiled_radius;

    x_start = Validate::range<int>(x_start, 0, texture->getWidth() - 1);
    x_end = Validate::range<int>(x_end, 0, texture->getWidth() - 1);

    z_start = Validate::range<int>(z_start, 0, texture->getHeight() - 1);
    z_end = Validate::range<int>(z_end, 0, texture->getHeight() - 1);

    for (auto x = x_start; x <= x_end; ++x)
    {
        for (auto z = z_start; z <= z_end; ++z)
        {
            // Check if points is inside visibility range.
            auto distance = (index.first - x) * (index.first - x) + (index.second - z) * (index.second - z);

            if (distance > ceiled_radius * ceiled_radius)
            {
                continue;
            }

            // Update visibility map.
            if (reveal)
            {
                unit_position_layer[x][z].visibility_count += 1;
            }
            else
            {
                unit_position_layer[x][z].visibility_count -= 1;
            }

            // Update texture.
            if (unit_position_layer[x][z].visibility_count > 0)
            {
                std::memcpy(&destination_buffer[z * texture_pitch + (x * texture_depth)], fog_of_war_reveal_color, 4);
            }
            else
            {
                std::memcpy(&destination_buffer[z * texture_pitch + (x * texture_depth)], fog_of_war_hide_color, 4);
            }
        }
    }

    pixel_buffer->unlock();

    if (get_id() == Game::getSingleton().get_active_player_id())
    {
        apply_fog_of_war();
    }
}
