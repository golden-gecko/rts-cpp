#include "Terrain.hpp"

#include "Exception.hpp"
#include "Game.hpp"
#include "Log.hpp"
#include "Objects/Object.hpp"
#include "Utils.hpp"

namespace Gecko
{
    namespace LayerType
    {
        std::string to_string(Value value)
        {
            auto i = type_to_string.left.find(value);

            if (i == type_to_string.left.end())
            {
                throw Exception("Value not found.");
            }

            return i->second;
        }

        Value from_string(const std::string& value)
        {
            auto i = type_to_string.right.find(value);

            if (i == type_to_string.right.end())
            {
                throw Exception("Value not found.");
            }

            return i->second;
        }
    }

    TerrainMesh::TerrainMesh()
    {
        operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
        materialName = "white";
    }

    void TerrainMesh::init()
    {
        scene_node = Game::getSingleton().create_scene_node();
        scene_node->getUserObjectBindings().setUserAny(Ogre::Any(this));

        if (vertices.size() <= 0)
        {
            // throw Exception("1");
            return;
        }

        if (colors.size() > 0 && colors.size() != vertices.size())
        {
            // throw Exception("2");
            return;
        }

        /*
        if (textureCoordinates.size() > 0 && textureCoordinates.size() != vertices.size())
        {
            // throw Exception("3");
            return;
        }
        */

        if (normals.size() > 0 && normals.size() != vertices.size())
        {
            // throw Exception("4");
            return;
        }

        const auto manualObject = Game::getSingleton().create_manual_object();

        manualObject->begin(materialName, operationType);

        for (std::vector<Ogre::Vector3>::size_type i = 0; i < vertices.size(); i++)
        {
            manualObject->position(vertices[i]);

            if (colors.size() > 0)
            {
                manualObject->colour(colors[i]);
            }

            for (auto j = 0; j < textureCoordinates.size(); j++)
            {
                if (textureCoordinates[j].size() > 0)
                {
                    manualObject->textureCoord(textureCoordinates[j][i]);
                }
            }

            if (normals.size() > 0)
            {
                manualObject->normal(normals[i]);
            }
        }

        for (auto& index: indices)
        {
            manualObject->index(index);
        }

        // TODO: Read from configuration. Can cause conflict.
        const auto mesh_name = "terrain_" + Convert::to_string(Random::get_int(1000, 9000));

        manualObject->end();
        manualObject->convertToMesh(mesh_name);

        Game::getSingleton().destroy_manual_object(manualObject);

        entity = Game::getSingleton().create_entity(mesh_name);
        entity->getUserObjectBindings().setUserAny(Ogre::Any(this));
        entity->setQueryFlags(Object::QueryFlags::QF_TERRAIN);

        scene_node->attachObject(entity);
        scene_node->getUserObjectBindings().setUserAny(Ogre::Any(this));
    }

    void TerrainLayerOccupied::update(float time)
    {
        if (changed == false)
        {
            return;
        }

        auto texture = Ogre::TextureManager::getSingleton().load(source, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Ogre::HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();
        pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
        const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();
        Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

        for (int x = 0; x < get_size(); x++)
        {
            for (int z = 0; z < get_size(); z++)
            {
                auto value = std::clamp<std::uint16_t>(get_data(x, z), 0, 10);
                auto offset = z * get_size() * 4 + x * 4;

                // b
                // g
                // r
                // a

                pDest[offset    ] = 0;
                pDest[offset + 1] = 127 - value * 127 / 10;
                pDest[offset + 2] = 127 + value * 128 / 10;
                pDest[offset + 3] = value > 0 ? 255 : 0;
            }
        }

        pixelBuffer->unlock();

        changed = false;
    }

    void TerrainLayerPollutionAir::update(float time)
    {
        if (changed == false)
        {
            return;
        }

        auto texture = Ogre::TextureManager::getSingleton().load(source, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Ogre::HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();
        pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
        const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();
        Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

        for (int x = 0; x < get_size(); x++)
        {
            for (int z = 0; z < get_size(); z++)
            {
                auto value = std::clamp<std::uint16_t>(get_data(x, z), 0, 10);
                auto offset = z * get_size() * 4 + x * 4;

                // b
                // g
                // r
                // a

                pDest[offset    ] = 0;
                pDest[offset + 1] = 127 - value * 127 / 10;
                pDest[offset + 2] = 127 + value * 128 / 10;
                pDest[offset + 3] = value > 0 ? 255 : 0;

                /*
                // @todo Hardcoded value.
                if (value > 10)
                {
                    value = 10;
                }

                // @todo Add color change method.
                if (size > 0.1f)
                {
                    // Colors changes from white (255, 255, 255), to yellow (255, 255, 0) and then to red (255, 0, 0).
                    if (size >= 50.0f)
                    {
                        pDest[z * get_size() * 4 + x * 4 + 0] = 0;
                        pDest[z * get_size() * 4 + x * 4 + 1] = 255 - (((value - 50.0f) / 50.0f) * 255);
                        pDest[z * get_size() * 4 + x * 4 + 2] = 255;
                        pDest[z * get_size() * 4 + x * 4 + 3] = 255;
                    }
                    else
                    {
                        pDest[z * get_size() * 4 + x * 4 + 0] = 255 - ((value / 50.0f) * 255);
                        pDest[z * get_size() * 4 + x * 4 + 1] = 255;
                        pDest[z * get_size() * 4 + x * 4 + 2] = 255;
                        pDest[z * get_size() * 4 + x * 4 + 3] = 255;
                    }
                }
                else
                {
                    pDest[z * get_size() * 4 + x * 4 + 0] = 0;
                    pDest[z * get_size() * 4 + x * 4 + 1] = 0;
                    pDest[z * get_size() * 4 + x * 4 + 2] = 0;
                    pDest[z * get_size() * 4 + x * 4 + 3] = 0;
                }
                */
            }
        }

        pixelBuffer->unlock();

        changed = false;
    }

    void TerrainLayerTraffic::update(float time)
    {
        if (changed == false)
        {
            return;
        }

        auto texture = Ogre::TextureManager::getSingleton().load(source, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Ogre::HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();
        pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
        const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();
        Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

        for (int x = 0; x < get_size(); x++)
        {
            for (int z = 0; z < get_size(); z++)
            {
                auto value = std::clamp<std::uint16_t>(get_data(x, z), 0, 10);
                auto offset = z * get_size() * 4 + x * 4;

                // b
                // g
                // r
                // a

                pDest[offset    ] = 0;
                pDest[offset + 1] = 127 - value * 127 / 10;
                pDest[offset + 2] = 127 + value * 128 / 10;
                pDest[offset + 3] = value > 0 ? 255 : 0;

                /*
                // @todo Hardcoded value.
                if (value > 10)
                {
                    value = 10;
                }

                // @todo Add color change method.
                if (size > 0.1f)
                {
                    // Colors changes from white (255, 255, 255), to yellow (255, 255, 0) and then to red (255, 0, 0).
                    if (size >= 50.0f)
                    {
                        pDest[z * get_size() * 4 + x * 4 + 0] = 0;
                        pDest[z * get_size() * 4 + x * 4 + 1] = 255 - (((value - 50.0f) / 50.0f) * 255);
                        pDest[z * get_size() * 4 + x * 4 + 2] = 255;
                        pDest[z * get_size() * 4 + x * 4 + 3] = 255;
                    }
                    else
                    {
                        pDest[z * get_size() * 4 + x * 4 + 0] = 255 - ((value / 50.0f) * 255);
                        pDest[z * get_size() * 4 + x * 4 + 1] = 255;
                        pDest[z * get_size() * 4 + x * 4 + 2] = 255;
                        pDest[z * get_size() * 4 + x * 4 + 3] = 255;
                    }
                }
                else
                {
                    pDest[z * get_size() * 4 + x * 4 + 0] = 0;
                    pDest[z * get_size() * 4 + x * 4 + 1] = 0;
                    pDest[z * get_size() * 4 + x * 4 + 2] = 0;
                    pDest[z * get_size() * 4 + x * 4 + 3] = 0;
                }
                */
            }
        }

        pixelBuffer->unlock();

        changed = false;
    }

    Terrain::Terrain(const Configuration& configuration)
    {
        // TODO: Remove.
        materialName = configuration.get_string("material");
        material = Utils::load_material(configuration.get_string("material"));
        scale = configuration.get_vector3("scale");

        // Load layers.
        auto layers_configuration = configuration.get_child("layers");

        for (auto i = layers_configuration.begin(); i != layers_configuration.end(); ++i)
        {
            auto type = i->get("type", "").asString();

            if (type == "Height")
            {
                auto layer = std::make_unique<TerrainLayerHeight>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::Height, std::move(layer));
            }
            else if (type == "Navigation")
            {
                auto layer = std::make_unique<TerrainLayerNavigation>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::Navigation, std::move(layer));
            }
            else if (type == "Occupied")
            {
                auto layer = std::make_unique<TerrainLayerOccupied>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::Occupied, std::move(layer));
            }
            else if (type == "None")
            {
                auto layer = std::make_unique<TerrainLayerNone>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::None, std::move(layer));
            }
            else if (type == "PollutionAir")
            {
                auto layer = std::make_unique<TerrainLayerPollutionAir>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::PollutionAir, std::move(layer));
            }
            else if (type == "PollutionGround")
            {
                auto layer = std::make_unique<TerrainLayerPollutionGround>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::PollutionGround, std::move(layer));
            }
            else if (type == "PollutionWater")
            {
                auto layer = std::make_unique<TerrainLayerPollutionWater>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::PollutionWater, std::move(layer));
            }
            else if (type == "Roads")
            {
                auto layer = std::make_unique<TerrainLayerRoads>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::Roads, std::move(layer));
            }
            else if (type == "Traffic")
            {
                auto layer = std::make_unique<TerrainLayerTraffic>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::Traffic, std::move(layer));
            }
            else if (type == "Type")
            {
                auto layer = std::make_unique<TerrainLayerType>(*i);
                layer->load(*i);
                layers.emplace(LayerType::Value::Type, std::move(layer));
            }
            else
            {
                throw Exception("Unknown terrain layer type " + type);
            }
        }

        // Load heightmap.
        auto size = get_size() + 1;

        vertices.reserve(size * size);

        const int coordinates = 2;

        textureCoordinates.resize(coordinates);

        for (int i = 0; i < coordinates; ++i)
        {   
            textureCoordinates[i].reserve(size * size);
        }

        normals.reserve(size * size);

        for (auto z = 0; z < size; ++z)
        {
            for (auto x = 0; x < size; ++x)
            {
                // Compute height based on neighbours.
                int indices[4][2] =
                {
                    { -1, -1 },
                    {  0, -1 },
                    { -1,  0 },
                    {  0,  0 }
                };

                int count = 0;
                float height = 0.0f;

                for (int i = 0; i < 4; ++i)
                {
                    int xx = x + indices[i][0];
                    int zz = z + indices[i][1];

                    if (xx < 0) continue;
                    if (zz < 0) continue;

                    if (xx > size - 2) continue;
                    if (zz > size - 2) continue;

                    ++count;
                    height += static_cast<float>(layers.at(LayerType::Value::Height)->as<TerrainLayerHeight>()->get_data(xx, zz));
                }

                vertices.emplace_back(
                    static_cast<float>(x) * scale.x,
                    height / static_cast<float>(count) * scale.y,
                    static_cast<float>(z) * scale.z
                );

                textureCoordinates[0].emplace_back(
                    x / static_cast<float>(size - 1),
                    z / static_cast<float>(size - 1)
                );

                textureCoordinates[1].emplace_back(
                    static_cast<float>(x),
                    static_cast<float>(z)
                );

                normals.emplace_back(0.0f, 1.0f, 0.0f);
            }
        }

        indices.reserve((size - 1) * (size - 1) * 6);

        for (auto z = 0; z < size - 1; ++z)
        {
            for (auto x = 0; x < size - 1; ++x)
            {
                indices.emplace_back( z      * size + x    );
                indices.emplace_back((z + 1) * size + x    );
                indices.emplace_back( z      * size + x + 1);

                indices.emplace_back( z      * size + x + 1);
                indices.emplace_back((z + 1) * size + x    );
                indices.emplace_back((z + 1) * size + x + 1);
            }
        }
    }

    void Terrain::update(float time)
    {
        for (auto& [type, layer] : layers)
        {
            layer->update(time);
        }
    }

    void Terrain::show_grid(bool visible)
    {
        Ogre::TexturePtr texture;

        if (visible)
        {
            texture = Ogre::TextureManager::getSingleton().load("grid_yellow.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        }
        else
        {
            texture = Ogre::TextureManager::getSingleton().load("grid_empty.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        }

        material->getTechnique(0)->getPass(0)->getTextureUnitState("grid")->setTexture(texture);
    }

    bool Terrain::is_grid_visible() const
    {
        return material->getTechnique(0)->getPass(0)->getTextureUnitState("grid")->getTextureName() != "grid_empty.png";
    }

    Coordinate Terrain::validate_position(const Coordinate& position) const
    {
        return Coordinate(
            std::clamp<Index>(position.x, 0, get_size() - 1),
            std::clamp<Index>(position.z, 0, get_size() - 1)
        );
    }

    Ogre::Vector3 Terrain::validate_position(const Ogre::Vector3& position) const
    {
        auto x = std::clamp(position.x, 0.0f, get_size() * scale.x);
        auto z = std::clamp(position.z, 0.0f, get_size() * scale.z);

        return Ogre::Vector3(x, 0.0f, z);
    }

    Ogre::Vector3 Terrain::validate_position(const Ogre::Vector3& position, float min_height, float max_height) const
    {
        auto valid_position = position;
        auto terrain_position = get_position(valid_position.x, valid_position.z);

        valid_position.x = std::clamp(valid_position.x, scale.x, (get_size() - 1) * scale.x);
        valid_position.y = std::clamp(valid_position.y, terrain_position.y + min_height, terrain_position.y + max_height);
        valid_position.z = std::clamp(valid_position.z, scale.z, (get_size() - 1) * scale.z);

        return valid_position;
    }

    Ogre::Vector3 Terrain::get_position(float x, float z) const
    {
        auto ray_length = 1000.0f;

        auto position = validate_position(Ogre::Vector3(x, 0.0f, z));
        auto index = Utils::get_index_from_position(position, scale);

        auto x_index = index.x;
        auto z_index = index.z;

        auto x_rest = position.x - (x_index * scale.x);
        auto z_rest = position.z - (z_index * scale.z);

        auto ray = Ogre::Ray(Ogre::Vector3(position.x, ray_length, position.z), Ogre::Vector3::NEGATIVE_UNIT_Y);

        auto size = get_size() + 1;

        {
            Ogre::Vector3 p[3];

            p[0] = vertices[ z_index      * size + x_index    ];
            p[1] = vertices[(z_index + 1) * size + x_index    ];
            p[2] = vertices[ z_index      * size + x_index + 1];

            auto hit = Ogre::Math::intersects(ray, p[0], p[1], p[2], true, false);

            if (hit.first)
            {
                return Ogre::Vector3(position.x, ray_length - hit.second, position.z);
            }
        }

        {
            Ogre::Vector3 p[3];

            p[0] = vertices[ z_index      * size + x_index + 1];
            p[1] = vertices[(z_index + 1) * size + x_index    ];
            p[2] = vertices[(z_index + 1) * size + x_index + 1];

            auto hit = Ogre::Math::intersects(ray, p[0], p[1], p[2], true, false);

            if (hit.first)
            {
                return Ogre::Vector3(position.x, ray_length - hit.second, position.z);
            }
        }

        return Ogre::Vector3(position.x, 0.0f, position.z);
    }

    Ogre::Vector3 Terrain::snap_position_to_grid(const Ogre::Vector3& position, SnapType snap_type) const
    {
        auto index = get_index(position);
        auto scale = get_scale();

        static const std::map<SnapType, std::pair<float, float>> indices =
        {
            { SnapType::Center, { scale.x / 2.0f, scale.x / 2.0f } },
            { SnapType::LeftBottom, { 0.0f, 0.0f } },
            { SnapType::LeftTop, { 0.0f, scale.z, } },
            { SnapType::RightBottom, { scale.x, 0.0f } },
            { SnapType::RightTop, { scale.x, scale.z } }
        };

        auto i = indices.find(snap_type);

        if (i == indices.end())
        {
            L_WARNING << "Unknown snap type";

            return position;
        }

        return Ogre::Vector3(index.x * scale.x + i->second.first, position.y, index.z * scale.z + i->second.second);
    }

    Path::Points Terrain::get_path(const Ogre::Vector3& from, const Ogre::Vector3& to) const
    {
        return search_strategy->get_path(from, to);
    }

    void Terrain::set_search_strategy(Search::Type type)
    {
        switch (type)
        {
            case Search::Type::AvoidOccupied:
                search_strategy = std::make_unique<AvoidOccupiedSearch>(*this);
                break;

            case Search::Type::FollowRoads:
                search_strategy = std::make_unique<FollowRoadsSearch>(*this);
                break;

            default:
                L_WARNING << "Unknown search strategy type. Falling back to Direct.";

                search_strategy = std::make_unique<DirectLineSearch>(*this);
        }
    }
}
