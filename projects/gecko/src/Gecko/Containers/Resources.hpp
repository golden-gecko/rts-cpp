#pragma once

#include "Gecko/Interfaces/Serializable.hpp"
#include "Gecko/Interfaces/Updatable.hpp"
#include "Gecko/Resource.hpp"

namespace Gecko
{
    class Resources :
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
        typedef std::map<std::string, Resource> Map;

    public:
        float add(const std::string& name, float value);

        void clear()
        {
            max_storage = 0.0f;

            m_items.clear();
        }

        auto empty() const
        {
            return m_items.empty();
        }

        bool has_resource(const std::string& name) const;

        bool has_resource(const std::string& name, float value) const;

        bool has_storage(const std::string& name) const;

        bool has_storage(const std::string& name, float value) const;

        void merge(Resources& other);

        float remove(const std::string& name, float value);

        auto size() const
        {
            return m_items.size();
        }

    public:
        float get_current(const std::string& name) const;

        const auto& get_items() const
        {
            return m_items;
        }

        float get_max(const std::string& name) const
        {
            return m_items.find(name)->second.get_max();
        }

        auto get_max_storage() const
        {
            return max_storage;
        }

        float get_storage(const std::string& name) const;

    public:
        void set_max_storage(float max_storage)
        {
            this->max_storage = max_storage;
        }

    public:
        auto begin()
        {
            return m_items.begin();
        }

        auto end()
        {
            return m_items.end();
        }

        auto cbegin() const
        {
            return m_items.cbegin();
        }

        auto cend() const
        {
            return m_items.cend();
        }

    public:
        bool operator==(const Resources& other) const;

    private:
        float max_storage = 0.0f;
        Map m_items;

        Resource& get(const std::string& name)
        {
            return m_items.find(name)->second;
        }
    };
}
