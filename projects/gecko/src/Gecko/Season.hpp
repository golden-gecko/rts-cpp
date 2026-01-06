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
        explicit Season(const std::string& name, const Configuration& configuration);

    public:
        void apply();

    public:
        const auto& get_color() const
        {
            return m_color;
        }

        const auto& get_name() const
        {
            return m_name;
        }

    private:
        std::string m_name;
        Ogre::ColourValue m_color;
        Timer m_timer;
    };
}
