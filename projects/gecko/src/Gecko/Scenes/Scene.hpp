#pragma once

#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Scene :
        public Updatable
    {
    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Scene(const ConfigurationPtr& configuration);

        ~Scene() override;

    public:
        Ogre::Camera* create_camera(const std::string& name) const;
        void destroy_camera(Ogre::Camera* camera) const;

        Ogre::Entity* create_entity(const std::string& name) const;
        void destroy_entity(Ogre::Entity* entity) const;

        Ogre::ManualObject* create_manual_object() const;
        void destroy_manual_object(Ogre::ManualObject* manual_object) const;

        Ogre::PlaneBoundedVolumeListSceneQuery* create_plane_volume_query(const Ogre::PlaneBoundedVolumeList& volumes, Ogre::uint32 mask) const;
        Ogre::RaySceneQuery* create_ray_scene_query(const Ogre::Ray& ray, Ogre::uint32 query_mask) const;
        void destroy_scene_query(Ogre::SceneQuery* scene_query);

        Ogre::SceneNode* create_scene_node() const;
        void destroy_scene_node(Ogre::SceneNode* scene_node) const;

    public:
        std::shared_ptr<Camera> get_camera(const std::string& name) const
        {
            auto camera = m_cameras.find(name);

            if (camera == m_cameras.end())
            {
                return nullptr;
            }

            return camera->second;
        }

        const std::map<std::string, std::shared_ptr<Camera>>& get_cameras() const
        {
            return m_cameras;
        }

        Ogre::SceneManager* get_scene_manager() const
        {
            return m_scene_manager;
        }

    protected:
        Ogre::SceneManager* m_scene_manager    = nullptr;
        Ogre::Light*        m_light            = nullptr;
        Ogre::SceneNode*    m_light_scene_node = nullptr;

    private:
        std::map<std::string, std::shared_ptr<Camera>> m_cameras;

        void init_cameras(const ConfigurationPtr& configuration);
        void deinit_cameras();
    };
}
