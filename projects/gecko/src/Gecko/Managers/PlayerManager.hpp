#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Managers/Manager.hpp"

namespace Gecko
{
    class PlayerManager :
        public Ogre::Singleton<PlayerManager>,
        public Manager<Player, std::string, Id>,
        public Updatable
    {
    private:
        using base_type = Manager<Player, std::string, Id>;

    public:
        // From Initializable.
        void init();
        void deinit();

    public:
        // From Updatable.
        void update(float time) override;

    public:
        PlayerManager(const ConfigurationPtr& configuration);

    public:
        PlayerPtr get_by_configuration_name(const std::string& name) const;

    private:
        ConfigurationPtr m_configuration;
    };
}
