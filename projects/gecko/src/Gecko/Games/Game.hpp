#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class Game :
        public Ogre::Singleton<Game>,
        public Ogre::RenderQueueListener,
        public Updatable
    {
    public:
        // From Ogre::RenderQueueListener.
        void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& cameraName, bool& skipThisInvocation) override;

    public:
        explicit Game(const std::shared_ptr<Configuration>& configuration);

        virtual ~Game() = default;

        virtual void init();
        virtual void deinit();

    public:
        void load_map(const std::string& map_name);
        void load_save(const std::string& save_name);
        void quit();
        void run();
        void save();
        void shutdown();
        void update(float time) override;
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
            return root->getNextFrameNumber() - 1;
        }

        std::vector<std::string> get_maps() const;

        const auto& get_name() const
        {
            return name;
        }

        auto get_root() const
        {
            return root;
        }

        std::vector<std::string> get_saves() const;

        auto get_scene_manager() const
        {
            return scene_manager;
        }

        std::shared_ptr<Window> get_window(const std::string& name) const
        {
            auto window = windows.find(name);

            if (window == windows.end())
            {
                return nullptr;
            }

            return window->second;
        }

        const auto& get_windows() const
        {
            return windows;
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
        std::shared_ptr<Configuration> m_configuration;

        bool active = true;

        std::string name;

        Id active_map_id;
        Id active_player_id;

    // TODO: Make private.
    public:
        OgreBites::ApplicationContext* context = nullptr;

    private:
        Ogre::Root* root = nullptr;

        OgreBites::SGTechniqueResolverListener* material_listener = nullptr;

        Ogre::SceneManager* scene_manager = nullptr;

        Ogre::Light* light = nullptr;
        Ogre::SceneNode* light_scene_node = nullptr;

        std::map<std::string, std::shared_ptr<Window>> windows;

        void init_meshes();
        void init_resources();
        void init_root();
        void init_shader_system();
        void init_shader_system_cache();
        void init_scene();
        void init_windows();

        void deinit_maps();
        void deinit_resources();
        void deinit_root();
        void deinit_scene();
        void deinit_shader_system();
        void deinit_shader_system_cache();
        void deinit_windows();

        void process_events();
    };
}
