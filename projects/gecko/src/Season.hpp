#pragma once

#include "Interfaces/Initializable.hpp"
#include "Interfaces/Updatable.hpp"
#include "Timer.hpp"

namespace Gecko
{
    class Season :
        public Initializable,
        public Updatable
    {
    public:
        explicit Season(const std::string& name, const Configuration& configuration);

        void init() override;
        void deinit() override;

        void update(float time) override;

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
