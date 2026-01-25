#include "Gecko/Layers/Layer.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/DataLayers/DataLayer.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Navigation/Search/AvoidOccupiedSearch.hpp"
#include "Gecko/Navigation/Search/DirectLineSearch.hpp"
#include "Gecko/Navigation/Search/FollowRoadsSearch.hpp"
#include "Gecko/Validators/RoadPosition.hpp"
#include "Gecko/Validators/SquareCenterPosition.hpp"

namespace Gecko
{
    Layer::Layer(Map* owner, const std::string& name, const Configuration& configuration) :
        m_owner(owner),
        m_name(name)
    {
        deserialize_position_validator(configuration);
        deserialize_search(configuration);
    }

    std::optional<Path::Points> Layer::search(const Ogre::Vector3& from, const Ogre::Vector3& to) const
    {
        // TODO: Check.
        return m_search->get_path(from, to);
    }

    void Layer::show_data_layer(const std::string& data_layer_name)
    {
        auto data_layer = m_data_layers.find(data_layer_name);

        if (data_layer == m_data_layers.end())
        {
            throw Exception("Data layer '" + data_layer_name + "' not found.");
        }

        // TODO: Rename material to material_name.
        // Get material.
        auto _material = Ogre::MaterialManager::getSingleton().getByName(m_material_name);

        if (_material.isNull())
        {
            return;
        }

        // Set texture.
        for (const auto& technique : _material->getTechniques())
        {
            for (const auto& pass : technique->getPasses())
            {
                auto texture_unit_state = pass->getTextureUnitState("data");

                if (texture_unit_state)
                {
                    texture_unit_state->setTexture(data_layer->second->get_texture());
                }
            }
        }
    }

    Ogre::Vector3 Layer::validate_position(const Ogre::Vector3& position) const
    {
        return m_position_validator->validate(position);
    }

    std::shared_ptr<DataLayer> Layer::get_data_layer(const std::string& name) const
    {
        auto data_layer = m_data_layers.find(name);

        if (data_layer == m_data_layers.end())
        {
            throw Exception("No '" + name + "' data layer.");
        }

        return data_layer->second;
    }

    void Layer::deserialize_position_validator(const Configuration& configuration)
    {
        if (configuration.has_member("validators.position"))
        {
            auto position_validator_name = configuration.get_string("validators.position");

            if (position_validator_name == "SquareCenter")
            {
                m_position_validator = std::make_shared<SquareCenterPosition>(*this);
            }
            else if (position_validator_name == "Road")
            {
                m_position_validator = std::make_shared<RoadPosition>(*this);
            }
            else
            {
                throw Exception("Invalid position validator '" + position_validator_name + "' for layer '" + m_name + "'.");
            }
        }
        else
        {
            m_position_validator = std::make_shared<PositionValidator>(*this);
        }
    }

    void Layer::deserialize_search(const Configuration& configuration)
    {
        if (configuration.has_member("search.algorithm"))
        {
            auto search_name = configuration.get_string("search.algorithm");

            if (search_name == "AvoidOccupied")
            {
                m_search = std::make_shared<AvoidOccupiedSearch>(*this);
            }
            else if (search_name == "DirectLine")
            {
                m_search = std::make_shared<DirectLineSearch>(*this);
            }
            else if (search_name == "FollowRoads")
            {
                m_search = std::make_shared<FollowRoadsSearch>(*this);
            }
            else
            {
                throw Exception("Invalid search algorithm '" + search_name + "' for layer '" + m_name + "'.");
            }
        }
        else
        {
            m_search = std::make_shared<DirectLineSearch>(*this);
        }
    }
}
