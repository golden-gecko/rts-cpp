#include "Gecko/Objects/Object.hpp"

#include "Gecko/Components/Mesh.hpp"
#include "Gecko/Components/Shield.hpp"
#include "Gecko/Components/Weapon.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Containers/Components.hpp"
#include "Gecko/Containers/Configurations.hpp"
#include "Gecko/Containers/Layers.hpp"
#include "Gecko/Containers/Orders.hpp"
#include "Gecko/Containers/Processes.hpp"
#include "Gecko/Containers/Resources.hpp"
#include "Gecko/Containers/Skills.hpp"
#include "Gecko/DataLayers/DataLayer.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Managers/JobManager.hpp"
#include "Gecko/Managers/ObjectManager.hpp"
#include "Gecko/Managers/OrderManager.hpp"
#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/Orders/OrderWait.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Rectangle.hpp"
#include "Gecko/Scenes/Scene.hpp"
#include "Gecko/Skills/Skill.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/Mesh.hpp"
#include "Gecko/Utils/Node.hpp"

namespace Gecko
{
    Object* Object::create(Object* memory, const ConfigurationPtr& configuration, const ScenePtr& scene)
    {
        auto object = new (memory) Object(scene);

        object->deserialize(configuration);

        return object;
    }

    Object::Object(const ScenePtr& scene) :
        m_scene(scene)
    {
        m_components = std::make_shared<Components>();
        m_configurations = std::make_shared<Configurations>();
        m_layers = std::make_shared<Layers>();
        m_orders = std::make_shared<Orders>();
        m_processes = std::make_shared<Processes>();
        m_resources = std::make_shared<Resources>();
        m_skills = std::make_shared<Skills>();
    }

    Object::Object(const Object& other) :
        base_type(other)
    {
        m_scene = other.m_scene;
        m_name = other.m_name;
        m_player_id = other.m_player_id;
        m_alive_timer = other.m_alive_timer;

        m_selectable = other.m_selectable;
        m_selected = other.m_selected;
        m_visible = other.m_visible;

        m_layers = other.m_layers;

        m_components = std::make_shared<Components>(*other.m_components);
        m_configurations = std::make_shared<Configurations>(*other.m_configurations);
        m_layers = std::make_shared<Layers>(*other.m_layers);
        m_orders = std::make_shared<Orders>(*other.m_orders);
        m_processes = std::make_shared<Processes>(*other.m_processes);
        m_resources = std::make_shared<Resources>(*other.m_resources);
        m_skills = std::make_shared<Skills>(*other.m_skills);

        if (other.m_scene_node)
        {
            m_scene_node = Utils::Mesh::copy_scene_node(other.m_scene_node);
        }

        // TODO: Does not work, because scene node has to be created from new parent.
        // selection = std::make_unique<Mesh>(*other.selection);
        create_selection_mesh();

        // TODO: Remove virtual method from constructor.
        set_selected(false);
        set_visible(false);
    }

    Object::~Object()
    {
        if (m_scene)
        {
            m_scene->destroy_scene_node(m_scene_node);
        }
    }

    void Object::init()
    {
        base_type::init();

        m_orders->clear();

        m_alive_timer.reset();

        for (const auto& i : *(m_components))
        {
            i->set_owner(this);
            i->set_scene(m_scene);
            i->init();
        }

        for (const auto& i : *(m_skills))
        {
            i->set_owner(this);
            i->init();
        }

        set_selected(false);
        set_visible(true);

        /*
        if (type != Type::Value::Missile)
        {
            // map->get_terrain().set_unit_position(get_id(), get_position());

            auto player = PlayerManager::getSingleton().get(get_player_id());

            if (player)
            {
                player->set_unit_position(get_id(), get_position(), get_visibility_range());
            }
        }
        */
    }

    void Object::deinit()
    {
        for (const auto& i : *(m_components))
        {
            i->deinit();
        }

        set_selected(false);
        set_visible(false);

        base_type::deinit();
    }

    ConfigurationPtr Object::serialize() const
    {
        auto configuration = base_type::serialize();

        configuration->set("name", m_name);
        configuration->set("player_id", m_player_id);
        configuration->set("alive_timer", m_alive_timer.serialize());
        configuration->set("selectable", m_selectable);
        configuration->set("selected", m_selected);
        configuration->set("visible", m_visible);
        configuration->set("position", get_position());

        configuration->append("components", m_components->serialize());
        configuration->append("configurations", m_configurations->serialize());
        configuration->append("layers", m_layers->serialize());
        configuration->append("orders", m_orders->serialize());
        configuration->append("processes", m_processes->serialize());
        configuration->append("resources", m_resources->serialize());
        configuration->append("skills", m_skills->serialize());

        return configuration;
    }

    void Object::deserialize(const ConfigurationPtr& configuration)
    {
        base_type::deserialize(configuration);

        m_name = configuration->get_string("name");
        m_player_id = configuration->get_int("player_id", Id::Empty.get());

        if (configuration->has_member("alive_timer"))
        {
            m_alive_timer.deserialize(configuration->get_child("alive_timer"));
        }

        m_selectable = configuration->get_bool("selectable", false);
        m_selected = configuration->get_bool("selected", false);
        m_visible = configuration->get_bool("visible", false);

        if (configuration->has_member("components"))
        {
            m_components->deserialize(configuration->get_child("components"));
        }

        if (configuration->has_member("configurations"))
        {
            m_configurations->deserialize(configuration->get_child("configurations"));
        }

        if (configuration->has_member("layers"))
        {
            m_layers->deserialize(configuration->get_child("layers"));
        }

        if (configuration->has_member("orders"))
        {
            m_orders->deserialize(configuration->get_child("orders"));
        }

        if (configuration->has_member("processes"))
        {
            m_processes->deserialize(configuration->get_child("processes"));
        }

        if (configuration->has_member("resources"))
        {
            m_resources->deserialize(configuration->get_child("resources"));
        }

        if (configuration->has_member("skills"))
        {
            m_skills->deserialize(configuration->get_child("skills"));
        }

        m_scene->destroy_scene_node(m_scene_node);
        
        m_scene_node = m_scene->create_scene_node();
        m_scene_node->setFixedYawAxis(true);

        create_selection_mesh();

        set_position(configuration->get_vector3("position", Ogre::Vector3::ZERO));
        set_selected(false);
        set_visible(false);
    }

    void Object::update(float time)
    {
        if (m_alive_timer.update(time))
        {
            auto order = OrderManager::getSingleton().order_destroy(get_id(), get_id());

            // TODO: Throw exception.

            get_orders()->add_last(order->get_id());
        }

        update_components(time);
        update_orders(time);
        update_processes(time);
        update_resources(time);
        update_skills(time);

        if (get_orders()->empty() && m_job_timer.update(time))
        {
            auto jobs = JobManager::getSingleton().get_job(get_id(), get_components(), get_resources());

            if (jobs.empty() == false)
            {
                for (const auto& job : jobs)
                {
                    get_orders()->add_last(job->get_id());
                }

                m_job_timer.update(time);
            }
        }
    }

    Ogre::Vector3 Object::get_direction() const
    {
        return Utils::Node::get_direction(m_scene_node);
    }

    Entrance Object::get_entrance() const
    {
        LayerPtr layer = m_owner->get_layer(Settings::Layer::Terrain);

        if (layer == nullptr)
        {
            throw Exception("No '" + Settings::Layer::Terrain + "' layer.");
        }
        
        DataLayerPtr data_layer = layer->get_data_layer(Settings::Layer::Navigation);

        if (data_layer == nullptr)
        {
            throw Exception("No '" + Settings::Layer::Navigation + "' data layer.");
        }

        Navigation::Coordinate index = layer->get_index(get_position());

        std::array<Navigation::Coordinate, 4> indices =
        {
            Navigation::Coordinate(-1, 0,  0),
            Navigation::Coordinate( 0, 0,  1),
            Navigation::Coordinate( 0, 0, -1),
            Navigation::Coordinate( 1, 0,  0)
        };

        std::list<Navigation::Coordinate> open;
        std::list<Navigation::Coordinate> closed;

        Navigation::Coordinate adjacent(-1, -1);

        for (int i = 0; i < indices.size(); i++)
        {
            int x = index.x;
            int z = index.z;

            int xx = x + indices[i].x;
            int zz = z + indices[i].z;

            if (layer->is_index_valid(xx, zz) == false)
            {
                continue;
            }

            adjacent.x = xx;
            adjacent.z = zz;

            if (std::ranges::find(closed, adjacent) != closed.end())
            {
                continue;
            }

            if (data_layer->get_data(xx, zz) == 0)
            {
                break;
            }

            if (std::ranges::find(open, adjacent) == open.end())
            {
                open.push_back(adjacent);
            }
        }

        const Ogre::Vector3& scale = layer->get_scale();
        Ogre::Vector3 entrance = Ogre::Vector3(adjacent.x * scale.x, 0.0f, adjacent.z * scale.z);

        return Entrance(layer->get_position(entrance), get_direction());
    }

    float Object::get_heading() const
    {
        Ogre::Vector3 forward = Ogre::Vector3::NEGATIVE_UNIT_Z;
        Ogre::Vector3 normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
        Ogre::Vector3 direction = get_direction();
        float angle = std::acos(forward.dotProduct(direction));

        if (normal.dotProduct(forward.crossProduct(direction)) < 0.0f)
        {
            angle = -angle;
        }

        angle = Ogre::Math::RadiansToDegrees(angle);

        if (angle < 0.0f)
        {
            angle = angle + 360.0f;
        }

        return angle;
    }

    ConfigurationPtr Object::get_info() const
    {
        ConfigurationPtr info = std::make_shared<Configuration>();

        info->set("ID", get_id());
        info->set("Name", get_name());
        info->set("Player", PlayerManager::getSingleton().get(get_player_id())->get_name());
        info->set("Position", get_position());
        info->set("Heading", get_heading());

        ConfigurationPtr info_resources = std::make_shared<Configuration>();

        for (const auto& [name, resouce] : *(m_resources))
        {
            info_resources->set(name, Utils::Convert::to_string(resouce.get_current()) + "/" + Utils::Convert::to_string(resouce.get_max()));
        }

        info->set("Resources", info_resources);

        return info;
    }

    Area Object::get_area() const
    {
        const Ogre::AxisAlignedBox& aabb = m_scene_node->_getWorldAABB();
        LayerPtr layer = m_owner->get_layer(Settings::Layer::Terrain); // TODO: Hardcoded.

        return Area(); // layer->get_index(aabb.getMinimum()), layer->get_index(aabb.getMaximum()));
    }

    Ogre::Vector3 Object::get_size() const
    {
        return m_scene_node->_getWorldAABB().getSize();
    }

    void Object::set_player_id(const Id& player_id)
    {
        /*
        auto player = PlayerManager::getSingleton().get(get_player_id());

        if (player)
        {
            player->remove_unit_from_position(get_id(), get_position(), get_visibility_range());
        }
        */

        m_player_id = player_id;

        /*
        player = PlayerManager::getSingleton().get(get_player_id());

        if (player)
        {
            player->set_unit_position(get_id(), get_position(), get_visibility_range());

            // selection_entity->setMaterialName(player->get_color());
        }
        */

        // Set color based on player color.
        auto player = PlayerManager::getSingleton().get(get_player_id());

        if (player)
        {
            m_selection->get_entity().setMaterialName("selection_" + player->get_color());
        }
    }

    void Object::set_position(const Ogre::Vector3& position, bool validate)
    {
        auto old_position = get_position();

        if (m_owner && m_layers->size())
        {
            // TODO: Hardcoded.
            const auto& layer = get_owner()->get_layer(Settings::Layer::Terrain);
            auto new_position = layer->get_position(position, validate);

            auto normal = layer->get_normal(position);
            normal.normalise();

            auto current_direction = get_direction();
            current_direction.normalise();

            auto side_direction = current_direction.crossProduct(Ogre::Vector3::UNIT_Y);
            side_direction.normalise();

            Ogre::Degree pitch_angle = normal.angleBetween(current_direction);
            Ogre::Degree roll_angle = normal.angleBetween(side_direction);

            m_scene_node->resetOrientation();
            m_scene_node->setPosition(new_position);
            m_scene_node->setDirection(current_direction);
            m_scene_node->pitch(pitch_angle - Ogre::Degree(90.0f));
            m_scene_node->roll(roll_angle - Ogre::Degree(90.0f));
        }
        else
        {
            auto direction = get_direction();

            m_scene_node->resetOrientation();
            m_scene_node->setPosition(position);
            m_scene_node->setDirection(direction * Ogre::Vector3(1.0f, 0.0f, 1.0f));
        }

        auto new_position = get_position();

        if (m_owner && m_layers->size())
        {
            // TODO: Hardcoded.
            auto layer = m_owner->get_layer(Settings::Layer::Terrain);
            auto data_layer = layer->get_data_layer(Settings::Layer::Navigation);

            m_scene_node->_update(true, true);
            m_scene_node->_updateBounds();
            const auto& aabb = m_scene_node->_getWorldAABB();

            auto old_index = layer->get_index(old_position);
            auto new_index = layer->get_index(new_position);

            if (old_index != new_index)
            {
                data_layer->remove(old_index, Area(), 1);
                data_layer->add(new_index, Area(), 1);
            }
        }
    }

    const Ogre::Vector3& Object::get_position() const
    {
        return Utils::Node::get_position(m_scene_node);
    }

    std::map<std::string, float> Object::get_progress_bars() const
    {
        std::map<std::string, float> progress_bars;

        float health = 0.0f;
        float max_health = 0.0f;

        for (Components::Items::const_iterator i = m_components->cbegin(); i != m_components->cend(); i++)
        {
            health += (*i)->get_health();
            max_health += (*i)->get_max_health();
        }

        if (max_health <= 0.0f)
        {
            progress_bars["health"] = 0.0f;
        }
        else
        {
            progress_bars["health"] = std::roundf(health / max_health * 100.f);
        }

        return progress_bars;
    }

    void Object::set_selected(bool selected)
    {
        if (is_selectable() && selected)
        {
            m_selection->set_visible(true);
        }
        else
        {
            m_selection->set_visible(false);
        }

        m_selected = selected;
    }

    void Object::set_visible(bool visible)
    {
        m_scene_node->setVisible(visible);

        if (visible)
        {
            m_selection->set_visible(m_selected);
        }

        m_visible = visible;
    }

    void Object::update_components(float time)
    {
        for (const auto& i : *(m_components))
        {
            i->update(time);
        }
    }

    void Object::update_orders(float time)
    {
        if (get_orders()->empty())
        {
            return;
        }

        // Get order.
        auto order = OrderManager::getSingleton().get(get_orders()->front());

        if (order == nullptr)
        {
            L_WARNING << "Could not find order " << get_orders()->front() << ".";

            return;
        }

        // Get handler for order.
        const std::map<order_type::Value, std::function<OrderStatus(Order*, float)>> handlers =
        {
            { order_type::Value::Destroy, std::bind(&Object::on_destroy, this, std::placeholders::_1, std::placeholders::_2) },
            { order_type::Value::Follow, std::bind(&Object::on_follow, this, std::placeholders::_1, std::placeholders::_2) },
            { order_type::Value::Guard, std::bind(&Object::on_guard, this, std::placeholders::_1, std::placeholders::_2) },
            { order_type::Value::Load, std::bind(&Object::on_load, this, std::placeholders::_1, std::placeholders::_2) },
            { order_type::Value::Move, std::bind(&Object::on_move, this, std::placeholders::_1, std::placeholders::_2) },
            { order_type::Value::Patrol, std::bind(&Object::on_patrol, this, std::placeholders::_1, std::placeholders::_2) },
            { order_type::Value::Stop, std::bind(&Object::on_stop, this, std::placeholders::_1, std::placeholders::_2) },
            { order_type::Value::Unload, std::bind(&Object::on_unload, this, std::placeholders::_1, std::placeholders::_2) },
            { order_type::Value::Wait, std::bind(&Object::on_wait, this, std::placeholders::_1, std::placeholders::_2) }
        };

        auto handler = handlers.find(order->get_type());

        if (handler == handlers.end())
        {
            L_WARNING << "Could not find handler for order " << order_type::to_string(order->get_type()) << ".";

            return;
        }

        // Process order.
        switch (handler->second(order, time))
        {
            case OrderStatus::complete:
            {
                std::stringstream stream;

                stream << "Object " << get_id() << " has completed " << order_type::to_string(order->get_type()) << " order.";

                UI::getSingleton().log_info(stream.str(), get_id());

                get_orders()->remove(order->get_id());

                break;
            }

            case OrderStatus::complete_and_retry:
            {
                std::stringstream stream;

                stream << "Object " << get_id() << " has completed " << order_type::to_string(order->get_type()) << " order. Moving order to queue end.";

                UI::getSingleton().log_info(stream.str(), get_id());

                get_orders()->remove(order->get_id());
                get_orders()->add_last(order->get_id());

                break;
            }

            case OrderStatus::fail:
            {
                std::stringstream stream;

                stream << "Object " << get_id() << " failed to complete " << order_type::to_string(order->get_type()) << " order.";

                UI::getSingleton().log_info(stream.str(), get_id());

                get_orders()->remove(order->get_id());

                break;
            }

            case OrderStatus::in_progress:
            {
                break;
            }

            case OrderStatus::retry:
            {
                if (order->has_reached_max_attempts_to_complete())
                {
                    std::stringstream stream;

                    stream << "Object " << get_id() << " cannot currently complete " << order_type::to_string(order->get_type()) << " order.";

                    UI::getSingleton().log_info(stream.str(), get_id());

                    order->increase_attempts_to_complete();

                }
                else
                {
                    get_orders()->remove(order->get_id());
                }

                break;
            }
        }
    }

    void Object::update_processes(float time)
    {
        get_processes()->update(time, get_id(), get_position(), get_resources());
    }

    void Object::update_resources(float time)
    {
        get_resources()->update(time);

        /*
        TODO: Fix.
        if (resources->has_resource("Pollution"))
        {
            auto& terrain = ;
            auto layer = terrain.get_layer(LayerType::Value::PollutionAir)->as<TerrainLayerPollutionAir>();
            auto index = terrain.get_index(get_position());

            layer->add(index, resources->get_resource("Pollution").extract_all());
        }
        */
    }

    void Object::update_skills(float time)
    {
        for (const auto& i : *(m_skills))
        {
            i->update(time);
        }
    }

    OrderStatus Object::on_attack(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_create(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_destroy(Order* order, float time)
    {
        ObjectManager::getSingleton().destroy(get_id());

        return OrderStatus::complete;
    }

    OrderStatus Object::on_disable(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_enable(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_follow(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_guard(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_load(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_move(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_patrol(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_rally(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_stop(Order* order, float time)
    {
        get_orders()->remove_all_orders();

        return OrderStatus::complete;
    }

    OrderStatus Object::on_unload(Order* order, float time)
    {
        return OrderStatus::complete;
    }

    OrderStatus Object::on_wait(Order* order, float time)
    {
        auto wait_order = dynamic_cast<OrderWait*>(order);

        if (wait_order == nullptr)
        {
            L_WARNING << "Order not exists or has invalid type.";

            return OrderStatus::fail;
        }

        if (wait_order->get_wait_timer().update(time))
        {
            return OrderStatus::complete;
        }

        return OrderStatus::in_progress;
    }

    void Object::create_selection_mesh()
    {
        m_scene_node->_update(true, true);
        m_scene_node->_updateBounds();

        auto selection_scale = m_scene_node->_getWorldAABB().getSize() * Settings::UI::SelectionScale;
        selection_scale = Ogre::Vector3(2, 1, 2);

        m_selection = std::make_unique<Mesh>();
        m_selection->set_owner(this);
        m_selection->set_scene(m_scene);
        m_selection->deserialize(ConfigurationManager::getSingleton().get("selection"));
        m_selection->init();
        m_selection->get_entity().setCastShadows(false);
        m_selection->get_scene_node().setScale(selection_scale);
        m_selection->set_position(Settings::UI::SelectionOffset);
        m_selection->set_visible(false);

        // Set color based on player color.
        auto player = PlayerManager::getSingleton().get(get_player_id());

        if (player)
        {
            m_selection->get_entity().setMaterialName("selection_" + player->get_color());
        }
    }
}
