#include "Gecko/Scenes/Scene.hpp"

#include "Gecko/Cameras/FreeCamera.hpp"
#include "Gecko/Cameras/MinimapCamera.hpp"
#include "Gecko/Cameras/ObjectFollowCamera.hpp"
#include "Gecko/Cameras/TopDownCamera.hpp"
#include "Gecko/Configuration.hpp"
#include "Gecko/Exception.hpp"
#include "Gecko/Games/Game.hpp"
#include "Gecko/Settings.hpp"

namespace Gecko
{
    void Scene::update(float time)
    {
        for (const auto& [_, camera] : m_cameras)
        {
            camera->update(time);
        }
    }

    Scene::Scene(const ConfigurationPtr& configuration)
    {
        Ogre::ColourValue fog_color = configuration->get_color("fog.color", Ogre::ColourValue::White);
        float fog_density = configuration->get_float("fog.density", 0.0f);
        float fog_start = configuration->get_float("fog.start", 0.0f);
        float fog_end = configuration->get_float("fog.end", 0.0f);

        m_scene_manager = Game::getSingleton().create_scene_manager();
        m_scene_manager->setAmbientLight(configuration->get_color("ambient.color", Ogre::ColourValue::White));
        m_scene_manager->setFog(Ogre::FogMode::FOG_EXP2, fog_color, fog_density, fog_start, fog_end);
        m_scene_manager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
        m_scene_manager->setSkyBox(true, configuration->get_string("sky.name"), configuration->get_float("sky.distance", 0.0f));

        m_light = m_scene_manager->createLight();
        m_light->setDiffuseColour(configuration->get_color("directional.diffuse.color", Ogre::ColourValue::White));
        m_light->setSpecularColour(configuration->get_color("directional.specular.color", Ogre::ColourValue::White));
        m_light->setType(Ogre::Light::LightTypes::LT_DIRECTIONAL);

        m_light_scene_node = create_scene_node();
        m_light_scene_node->attachObject(m_light);
        m_light_scene_node->setDirection(configuration->get_vector3("directional.direction"));

        init_cameras(configuration->get_child("cameras"));
    }

    Scene::~Scene()
    {
        deinit_cameras();

        Game::getSingleton().destroy_scene_manager(m_scene_manager);
    }

    Ogre::Camera* Scene::create_camera(const std::string& name) const
    {
        return m_scene_manager->createCamera(name);
    }

    void Scene::destroy_camera(Ogre::Camera* camera) const
    {
        if (camera)
        {
            m_scene_manager->destroyCamera(camera);
        }
    }

    Ogre::Entity* Scene::create_entity(const std::string& name) const
    {
        return m_scene_manager->createEntity(name);
    }

    void Scene::destroy_entity(Ogre::Entity* entity) const
    {
        if (entity)
        {
            m_scene_manager->destroyEntity(entity);
        }
    }

    Ogre::ManualObject* Scene::create_manual_object() const
    {
        return m_scene_manager->createManualObject();
    }

    void Scene::destroy_manual_object(Ogre::ManualObject* manual_object) const
    {
        if (manual_object)
        {
            m_scene_manager->destroyManualObject(manual_object);
        }
    }

    Ogre::PlaneBoundedVolumeListSceneQuery* Scene::create_plane_volume_query(const Ogre::PlaneBoundedVolumeList& volumes, Ogre::uint32 mask) const
    {
        return m_scene_manager->createPlaneBoundedVolumeQuery(volumes, mask);
    }

    void Scene::destroy_query(Ogre::SceneQuery* scene_query)
    {
        if (scene_query)
        {
            m_scene_manager->destroyQuery(scene_query);
        }
    }

    Ogre::RaySceneQuery* Scene::create_ray_scene_query(const Ogre::Ray& ray) const
    {
        auto ray_scene_query = m_scene_manager->createRayQuery(ray);

        ray_scene_query->setSortByDistance(true);

        return ray_scene_query;
    }

    void Scene::destroy_ray_scene_query(Ogre::RaySceneQuery* ray_scene_query)
    {
        if (ray_scene_query)
        {
            m_scene_manager->destroyQuery(ray_scene_query);
        }
    }

    Ogre::SceneNode* Scene::create_scene_node() const
    {
        return m_scene_manager->getRootSceneNode()->createChildSceneNode();
    }

    void Scene::destroy_scene_node(Ogre::SceneNode* scene_node) const
    {
        if (scene_node)
        {
            m_scene_manager->destroySceneNode(scene_node);
        }
    }

    void Scene::init_cameras(const ConfigurationPtr& configuration)
    {
        for (auto i = configuration->begin(); i != configuration->end(); i++)
        {
            auto camera_configuration = std::make_shared<Configuration>(*i);

            auto camera_name = camera_configuration->get_string("name");
            auto camera_type = camera_configuration->get_string("type");

            if (camera_type == "Free")
            {
                m_cameras.emplace(camera_name, std::make_shared<FreeCamera>(this, camera_name, camera_configuration));
            }
            else if (camera_type == "Minimap")
            {
                m_cameras.emplace(camera_name, std::make_shared<MinimapCamera>(this, camera_name, camera_configuration));
            }
            else if (camera_type == "ObjectFollow")
            {
                m_cameras.emplace(camera_name, std::make_shared<ObjectFollowCamera>(this, camera_name, camera_configuration));
            }
            else if (camera_type == "TopDown")
            {
                m_cameras.emplace(camera_name, std::make_shared<TopDownCamera>(this, camera_name, camera_configuration));
            }
            else
            {
                throw Exception("Unknown camera type '" + camera_type + "'.");
            }
        }
    }

    void Scene::deinit_cameras()
    {
        m_cameras.clear();
    }
}
