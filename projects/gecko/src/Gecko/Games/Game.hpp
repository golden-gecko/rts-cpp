#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Game :
        public Ogre::Singleton<Game>,
        public Ogre::RenderQueueListener,
        public OgreBites::ApplicationContext,
        public Initializable,
        public Updatable
    {
    public:
        // From Ogre::RenderQueueListener.
        void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation) override;

    public:
        // From OgreBites::ApplicationContext.
        void windowResized(Ogre::RenderWindow* rw) override;

    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Game(const ConfigurationPtr& configuration);

        virtual ~Game() = default;

    public:
        void load_map(const std::string& map_name);
        void load_save(const std::string& save_name);
        void quit();
        void run();
        void save();
        void shutdown();
        void unload_map();
        void update_input(float time);

        Ogre::Entity* create_entity(const std::string& name) const;
        void destroy_entity(Ogre::Entity* entity) const;

        Ogre::ManualObject* create_manual_object() const;
        void destroy_manual_object(Ogre::ManualObject* manual_object) const;

        Ogre::RaySceneQuery* create_ray_scene_query(const Ogre::Ray& ray) const;
        void destroy_ray_scene_query(Ogre::RaySceneQuery* ray_scene_query);

        Ogre::SceneNode* create_scene_node() const;
        void destroy_scene_node(Ogre::SceneNode* scene_node) const;

        void save_options(const std::string& options);
        void load_options();

    public:
        Player* get_active_player() const;

        auto get_active_player_id() const
        {
            return active_player_id;
        }

        const auto& get_configuration() const
        {
            return m_configuration;
        }

        auto get_frame_number() const
        {
            return getRoot()->getNextFrameNumber() - 1;
        }

        std::vector<std::string> get_maps() const;

        const auto& get_name() const
        {
            return name;
        }

        std::vector<std::string> get_saves() const;

        Ogre::SceneManager* get_scene_manager() const
        {
            return scene_manager;
        }

    public:
        void set_active_player_id(const Id& id);

    protected:
        virtual void init_components(std::size_t max_size);
        virtual void init_maps(std::size_t max_size);
        virtual void init_objects(std::size_t max_size);
        virtual void init_orders(std::size_t max_size);
        virtual void init_players(std::size_t max_size);
        virtual void init_skills(std::size_t max_size);

    private:
        ConfigurationPtr m_configuration;

        std::string name;

        Id active_map_id;
        Id active_player_id;

    private:
        Ogre::SceneManager* scene_manager = nullptr;

        Ogre::Light* light = nullptr;
        Ogre::SceneNode* light_scene_node = nullptr;

        void init_meshes();
        void init_root();
        void init_scene();

        void deinit_maps();
        void deinit_root();
        void deinit_scene();
    };
}
