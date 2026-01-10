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
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        explicit Technology() = default;

    public:
        bool is_locked() const
        {
            return locked;
        }

        void research()
        {
            in_research = true;
        }

        void unlock()
        {
            locked = false;
        }

    private:
        std::string name;

        std::map<std::string, float> costs;
        std::set<std::string> obsoletes;
        std::set<std::string> unlocks;

        Timer research_timer;

        bool in_research = false;
        bool locked = true;
    };
}
