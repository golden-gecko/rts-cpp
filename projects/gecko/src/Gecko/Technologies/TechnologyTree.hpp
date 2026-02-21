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
        void research(const std::string& name) const;
        void unlock(const std::string& name) const;

    public:
        bool is_locked(const std::string& name) const;

    private:
        Container m_tree;
    };
}
