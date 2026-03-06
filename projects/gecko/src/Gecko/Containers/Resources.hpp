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
        using Container = std::map<std::string, Resource>;

    public:
        // From Serializable.
        ConfigurationPtr serialize() const override;
        void deserialize(const ConfigurationPtr& configuration) override;

    public:
        // From Updatable.
        void update(float time) override;

    public:
        std::uint64_t add(const std::string& name, std::uint64_t value);

        void clear()
        {
            m_max_storage = 0;

            m_items.clear();
        }

        bool empty() const
        {
            return m_items.empty();
        }

        bool has_resource(const std::string& name) const;
        bool has_resource(const std::string& name, std::uint64_t value) const;

        bool has_storage(const std::string& name) const;
        bool has_storage(const std::string& name, std::uint64_t value) const;

        void merge(const Resources& other);

        std::uint64_t remove(const std::string& name, std::uint64_t value);

        Container::size_type size() const
        {
            return m_items.size();
        }

    public:
        std::uint64_t get_current(const std::string& name) const;

        const auto& get_items() const
        {
            return m_items;
        }

        std::uint64_t get_max(const std::string& name) const
        {
            return m_items.find(name)->second.get_max();
        }

        std::uint64_t get_max_storage() const
        {
            return m_max_storage;
        }

        std::uint64_t get_storage(const std::string& name) const;

    public:
        void set_max_storage(float max_storage)
        {
            m_max_storage = max_storage;
        }

    public:
        Container::iterator begin()
        {
            return m_items.begin();
        }

        Container::iterator end()
        {
            return m_items.end();
        }

        Container::const_iterator cbegin() const
        {
            return m_items.cbegin();
        }

        Container::const_iterator cend() const
        {
            return m_items.cend();
        }

    public:
        bool operator==(const Resources& other) const;

    private:
        Container m_items;

        std::uint64_t m_max_storage = 0;

        Resource& get(const std::string& name)
        {
            return m_items.find(name)->second;
        }
    };
}
