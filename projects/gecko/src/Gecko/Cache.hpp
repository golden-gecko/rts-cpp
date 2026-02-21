#pragma once

namespace Gecko
{
    template<typename Key, typename Value>
    class Cache
    {
    public:
        using Container = std::map<Key, Value>;

    public:
        explicit Cache() = default;

    public:
        void clear()
        {
            m_values.clear();
        }

    public:
        std::optional<Value> get(const Key& key) const
        {
            auto value = m_values.find(key);

            if (value == m_values.end())
            {
                return {};
            }

            return value->second;
        }

    public:
        void set(const Key& key, const Value& value)
        {
            m_values.emplace(std::make_pair(key, value));
        }

    private:
        Container m_values;
    };
}
