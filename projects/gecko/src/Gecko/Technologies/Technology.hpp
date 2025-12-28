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
        explicit Technology() = default;

        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

        void update(float time) override;

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
