#include "Gecko/Tiles/SquareTile.hpp"

#include "Gecko/Configuration.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Layers/Layer.hpp"
#include "Gecko/Maps/Map.hpp"
#include "Gecko/QueryFlags.hpp"
#include "Gecko/Scenes/Scene.hpp"
#include "Gecko/Settings.hpp"
#include "Gecko/Utils/Convert.hpp"
#include "Gecko/Utils/File.hpp"
#include "Gecko/Utils/Texture.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko
{
    void SquareTile::init()
    {
        create_mesh();
    }

    void SquareTile::deinit()
    {
        m_owner->get_owner()->get_scene()->destroy_entity(m_entity);
        m_owner->get_owner()->get_scene()->destroy_scene_node(m_scene_node);

        Ogre::MeshManager::getSingleton().remove(get_mesh_name());
    }

    std::string SquareTile::get_mesh_name() const
    {
        std::string map_name = Utils::File::to_file_name(m_owner->get_owner()->get_name());
        std::string layer_name = Utils::File::to_file_name(m_owner->get_name());

        return map_name
            + "_"
            + layer_name
            + "_tile_"
            + Utils::Convert::to_string(m_tile_position.x)
            + "_"
            + Utils::Convert::to_string(m_tile_position.z)
            + ".mesh";
    }

    void SquareTile::create_mesh()
    {
        const auto& material = m_owner->get_material_name();
        const auto& scale = m_owner->get_scale();
        const auto& grid_scale = m_owner->get_grid_scale();

        // Get meshes cache configuration.
        auto game_configuration = Game::getSingleton().get_configuration();
        auto meshes_path = Settings::Cache::MeshesPath;
        auto mesh_name = get_mesh_name();
        auto mesh_path = std::filesystem::path(meshes_path) / std::filesystem::path(mesh_name);

        if (std::filesystem::exists(mesh_path) == false)
        {
            auto size = m_owner->get_tile_size() + 1;

            auto start_z = m_tile_position.z * (size - 1);
            auto start_x = m_tile_position.x * (size - 1);

            auto manualObject = m_owner->get_owner()->get_scene()->create_manual_object();
            manualObject->begin(material);

            for (auto z = start_z; z < start_z + size; ++z)
            {
                for (auto x = start_x; x < start_x + size; ++x)
                {
                    manualObject->position(m_owner->get_vertex_data(x, z));

                    for (auto i = 0; i < m_owner->get_texture_coordinate_data(x, z).size(); i++)
                    {
                         manualObject->textureCoord(m_owner->get_texture_coordinate_data(x, z, i));
                    }

                    manualObject->normal(m_owner->get_normal_data(x, z));
                }
            }

            for (auto z = 0; z < size - 1; ++z)
            {
                for (auto x = 0; x < size - 1; ++x)
                {
                    manualObject->index( z      * size + x    );
                    manualObject->index((z + 1) * size + x    );
                    manualObject->index( z      * size + x + 1);

                    manualObject->index( z      * size + x + 1);
                    manualObject->index((z + 1) * size + x    );
                    manualObject->index((z + 1) * size + x + 1);
                }
            }

            manualObject->end();
            auto mesh = manualObject->convertToMesh(get_mesh_name());

            // Save mesh to cache.
            if (game_configuration->get_bool("options.cache.meshes.enabled", true))
            {
                Ogre::MeshSerializer serializer;

                if (std::filesystem::exists(meshes_path) == false)
                {
                    std::filesystem::create_directories(meshes_path);
                }

                serializer.exportMesh(mesh.get(), mesh_path.string());
            }

            // Destroy manual object.
            m_owner->get_owner()->get_scene()->destroy_manual_object(manualObject);
        }

        m_entity = m_owner->get_owner()->get_scene()->create_entity(mesh_name);
        m_entity->setCastShadows(false);
        m_entity->setQueryFlags(QueryFlags::QF_Layer);

        // Create scene node.
        m_scene_node = m_owner->get_scene_node().createChildSceneNode();
        m_scene_node->attachObject(m_entity);
    }
}
