#include "Gecko/Technologies/TechnologyTree.hpp"

#include "Gecko/Managers/ConfigurationManager.hpp"
#include "Gecko/Technologies/Technology.hpp"

template<> Gecko::TechnologyTree* Ogre::Singleton<Gecko::TechnologyTree>::msSingleton = nullptr;

namespace Gecko
{
    void TechnologyTree::init()
    {
        for (const auto& [name, configuration] : ConfigurationManager::getSingleton())
        {
            // TODO: Remove default value.
            auto type = configuration->get_string("type", "");

            if (type == "Technology")
            {
                auto techonology = std::make_shared<Technology>();

                techonology->deserialize((configuration));

                m_tree.emplace(name, techonology);
            }
        }
    }

    void TechnologyTree::deinit()
    {
    }

    void TechnologyTree::update(float time)
    {
        for (const auto& [name, technology] : m_tree)
        {
            technology->update(time);
        }
    }

    bool TechnologyTree::is_locked(const std::string& name) const
    {
        auto technology = m_tree.find(name);

        if (technology == m_tree.end())
        {
            return false;
        }

        return technology->second->is_locked();
    }

    void TechnologyTree::research(const std::string& name)
    {
        auto technology = m_tree.find(name);

        if (technology != m_tree.end())
        {
            technology->second->research();
        }
    }

    void TechnologyTree::unlock(const std::string& name)
    {
        auto technology = m_tree.find(name);

        if (technology != m_tree.end())
        {
            technology->second->unlock();
        }
    }
}
