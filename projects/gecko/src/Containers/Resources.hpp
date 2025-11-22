#pragma once

#include "Interfaces/Serializable.hpp"
#include "Interfaces/Updatable.hpp"
#include "Resource.hpp"

namespace Gecko
{
    class Resources :
        public Serializable,
        public Updatable
    {
    public:
        typedef std::map<std::string, Resource> Map;

    public:
        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

        void update(float time) override;

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
        auto begin() const
        {
            return m_items.begin();
        }

        auto end() const
        {
            return m_items.end();
        }

        auto cbegin() const
        {
            return m_items.cbegin();
        }

        auto cend()
        {
            return m_items.cend();
        }

    public:
        bool operator==(const Resources& other) const;

    private:
        float max_storage = 0.0f;
        Map m_items;
    };
}
