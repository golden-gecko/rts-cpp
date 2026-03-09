#pragma once

#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Timer.hpp"

namespace Gecko
{
    class Season :
        public Initializable,
        public Updatable
    {
    public:
        // From Initializable.
        void init() override;
        void deinit() override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Season(const std::string& name, const ConfigurationPtr& configuration);

    public:
        void apply();

    public:
        const std::string& get_name() const
        {
            return m_name;
        }

        const Ogre::ColourValue& get_color() const
        {
            return m_color;
        }

    private:
        std::string m_name;
        Ogre::ColourValue m_color;
        Timer m_timer;
    };
}
