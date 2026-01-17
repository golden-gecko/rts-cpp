#include "Gecko/Components/Mesh.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Objects/Object.hpp"
#include "Gecko/QueryFlags.hpp"
#include "Gecko/Utils/Mesh.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    Mesh* Mesh::create(Mesh* memory, const ConfigurationPtr& m_configuration)
    {
        auto component = new (memory) Mesh();

        component->deserialize(m_configuration);

        return component;
    }

    Mesh::Mesh(const Mesh& other) :
        base_type(other)
    {
        if (other.m_entity)
        {
            m_entity = Utils::Mesh::copy_entity(*other.m_entity);

            if (get_owner())
            {
                m_entity->getUserObjectBindings().setUserAny(Ogre::Any(get_owner()->get_id()));
            }
        }

        // TODO: Scene node is created at the same parent? Should be created from new owner.
        if (other.m_scene_node)
        {
            m_scene_node = Utils::Mesh::copy_scene_node(*other.m_scene_node);
            m_scene_node->attachObject(m_entity);
        }
    }

    Mesh::~Mesh()
    {
        // TODO: Fix.
        if (Game::getSingletonPtr())
        {
            Game::getSingleton().destroy_scene_node(m_scene_node);
            Game::getSingleton().destroy_entity(m_entity);
        }
    }

    Ogre::Vector3 Mesh::get_direction() const
    {
        return Utils::get_node_direction(*m_scene_node);
    }

    void Mesh::init()
    {
        base_type::init();

        if (m_configuration->has_member("mesh.name"))
        {
            load_from_file();
        }
        else if (m_configuration->has_member("mesh.vertices"))
        {
            load_from_vertices();
        }
        else
        {
            throw Exception("Mesh m_configuration has neither 'mesh.name' or 'mesh.vertices'.");
        }

        auto position = m_configuration->get_vector3("position", Ogre::Vector3::ZERO);
        auto scale = m_configuration->get_vector3("mesh.scale", Ogre::Vector3::UNIT_SCALE);

        m_scene_node = get_owner()->get_scene_node().createChildSceneNode();
        m_scene_node->attachObject(m_entity);
        m_scene_node->setFixedYawAxis(true);
        m_scene_node->setPosition(position);
        m_scene_node->setScale(scale);
    }

    void Mesh::set_owner(Object* _owner)
    {
        base_type::set_owner(_owner);

        if (m_entity)
        {
            m_entity->getUserObjectBindings().setUserAny(Ogre::Any(get_owner()->get_id()));
        }
    }

    void Mesh::set_position(const Ogre::Vector3& position)
    {
        m_scene_node->setPosition(position);
    }

    void Mesh::load_from_file()
    {
        m_entity = Game::getSingleton().create_entity(m_configuration->get_string("mesh.name"));
        m_entity->setQueryFlags(QueryFlags::QF_Object);
        m_entity->getUserObjectBindings().setUserAny(Ogre::Any(get_owner()->get_id()));
    }

    void Mesh::load_from_vertices()
    {
        auto game_configuration = Game::getSingleton().get_configuration();
        auto meshes_path = game_configuration->get_string("options.cache.meshes.path", Settings::Cache::MeshesPath);
        auto mesh_name = get_owner()->get_configuration()->get_name() + "_" + m_configuration->get_name() + ".mesh";
        auto mesh_path = std::filesystem::path(meshes_path) / std::filesystem::path(mesh_name);

        if (std::filesystem::exists(mesh_path) == false)
        {
            std::vector<Ogre::Vector3> vertices;
            std::vector<Ogre::ColourValue> colors;
            std::vector<std::vector<Ogre::Vector2>> texture_coordinates;
            std::vector<Ogre::Vector3> normals;
            std::vector<std::uint32_t> indices;

            // Load vertices.
            const auto model_vertices = m_configuration->get_child_optional("mesh.vertices");

            if (model_vertices)
            {
                if (model_vertices.value()->size() % 3 != 0)
                {
                    throw std::runtime_error("Vertices size must be multiple of 3.");
                }

                vertices.reserve(model_vertices.value()->size() / 3);

                auto j = model_vertices.value()->begin();

                for (size_t i = 0; i < model_vertices.value()->size(); i += 3)
                {
                    auto x = j->asFloat(); j++;
                    auto y = j->asFloat(); j++;
                    auto z = j->asFloat(); j++;

                    vertices.emplace_back(x, y, z);
                }
            }

            // Load colors.
            auto color = m_configuration->get_color("mesh.color", Ogre::ColourValue::White);
            colors.resize(vertices.size(), color);

            // Load texture coordinates.
            // TODO: Implement.

            // Load normals.
            const auto model_normals = m_configuration->get_child_optional("mesh.normals");

            if (model_normals)
            {
                if (model_normals.value()->size() % 3 != 0)
                {
                    throw std::runtime_error("Normals size must be multiple of 3.");
                }

                normals.reserve(model_normals.value()->size() / 3);

                auto j = model_normals.value()->begin();

                for (size_t i = 0; i < model_normals.value()->size(); i += 3)
                {
                    auto x = j->asFloat(); j++;
                    auto y = j->asFloat(); j++;
                    auto z = j->asFloat(); j++;

                    normals.emplace_back(x, y, z);
                }
            }

            // Load indices.
            const auto model_indices = m_configuration->get_child_optional("mesh.indices");

            if (model_indices)
            {
                indices.reserve(model_indices.value()->size());

                for (const auto& i : *(model_indices.value()))
                {
                    indices.emplace_back(static_cast<std::uint32_t>(i.asInt64()));
                }
            }

            if (vertices.empty())
            {
                throw Exception("Mesh has no vertices.");
            }

            if (vertices.size() != colors.size())
            {
                throw Exception("Colors has different size than vertices.");
            }

            if (vertices.size() != normals.size())
            {
                throw Exception("Normals has different size than vertices.");
            }

            for (auto i = 0; i < texture_coordinates.size(); i++)
            {
                if (vertices.size() != texture_coordinates[i].size())
                {
                    throw Exception("Texture coordinates has different size than vertices.");
                }
            }

            auto mesh = Game::getSingleton().create_manual_object();
            auto material_name = m_configuration->get_string("mesh.material_name", "white");

            mesh->begin(material_name);

            for (auto i = 0; i < vertices.size(); i++)
            {
                mesh->position(vertices[i]);
                mesh->colour(colors[i]);
                mesh->normal(normals[i]);

                // TODO: Implement.
                mesh->textureCoord(0.0f, 0.0f);

                for (auto j = 0; j < texture_coordinates.size(); ++j)
                {
                    mesh->textureCoord(texture_coordinates[j][i]);
                }
            }

            for (auto i : indices)
            {
                mesh->index(i);
            }

            mesh->end();
            auto _mesh = mesh->convertToMesh(mesh_name);

            // Save mesh to cache.
            if (game_configuration->get_bool("options.cache.meshes.enabled", true))
            {
                Ogre::MeshSerializer serializer;

                if (std::filesystem::exists(meshes_path) == false)
                {
                    std::filesystem::create_directories(meshes_path);
                }

                serializer.exportMesh(_mesh.get(), mesh_path.string());
            }

            // Destroy manual object.
            Game::getSingleton().destroy_manual_object(mesh);
        }

        m_entity = Game::getSingleton().create_entity(mesh_name);
        m_entity->setQueryFlags(QueryFlags::QF_Object);
        m_entity->getUserObjectBindings().setUserAny(Ogre::Any(get_owner()->get_id()));
    }
}
