#pragma once

#include "Interfaces/Initializable.hpp"
#include "Interfaces/Updatable.hpp"

namespace Gecko
{
    class TechnologyTree :
        public Ogre::Singleton<TechnologyTree>,
        public Initializable,
        public Updatable
    {
    public:
        explicit TechnologyTree() = default;

        void init() override;
        void deinit() override;

        void update(float time) override;

    public:
        bool is_locked(const std::string& name) const;

        void research(const std::string& name);

        void unlock(const std::string& name);

    private:
        std::map<std::string, std::shared_ptr<Technology>> m_tree;
    };
}
