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
        using Container = std::map<std::string, std::shared_ptr<Technology>>;

    // From Initializable.
    public:
        void init() override;
        void deinit() override;

    // From Updatable.
    public:
        void update(float time) override;

    // TechnologyTree.
    public:
        void research(const std::string& name) const;
        void unlock(const std::string& name) const;

    public:
        bool is_locked(const std::string& name) const;

    private:
        Container m_tree;
    };
}
