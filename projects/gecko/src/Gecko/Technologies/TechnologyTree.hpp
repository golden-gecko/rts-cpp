#pragma once

#include "Gecko/Interfaces/Initializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"

namespace Gecko
{
    class TechnologyTree :
        public Ogre::Singleton<TechnologyTree>,
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
        explicit TechnologyTree() = default;

    public:
        bool is_locked(const std::string& name) const;

        void research(const std::string& name);

        void unlock(const std::string& name);

    private:
        std::map<std::string, std::shared_ptr<Technology>> m_tree;
    };
}
