#pragma once

namespace Gecko
{
    template<typename Key, typename Value>
    class Cache
    {
    public:
        std::optional<Value> get(const Key& key) const
        {
            auto value = m_cache.find(key);

            if (value == m_cache.end())
            {
                return {};
            }

            return value->second;
        }

    public:
        void set(const Key& key, const Value& value)
        {
            m_cache.emplace(std::make_pair(key, value));
        }

    private:
        std::map<Key, Value> m_cache;
    };
}
