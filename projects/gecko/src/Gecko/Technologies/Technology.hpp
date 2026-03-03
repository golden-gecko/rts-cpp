#pragma once

#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Timer.hpp"

namespace Gecko
{
    class Technology :
        public Serializable,
        public Updatable
    {
    public:
        static const std::string Name;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        void research()
        {
            m_in_research = true;
        }

        void unlock()
        {
            m_locked = false;
        }

    public:
        bool is_locked() const
        {
            return m_locked;
        }

    private:
        std::string m_name;

        std::map<std::string, float> m_costs;
        std::set<std::string>        m_obsoletes;
        std::set<std::string>        m_unlocks;

        Timer m_research_timer;

        bool m_in_research = false;
        bool m_locked      = true;
    };
}
