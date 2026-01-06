#pragma once

#include "Gecko/Interfaces/Serializable.hpp"

namespace Gecko
{
    class Configurations :
        public Serializable
    {
    public:
        // From Serializable.
        std::shared_ptr<Configuration> serialize() const override;
        void deserialize(const std::shared_ptr<Configuration>& configuration) override;

    public:
        typedef std::set<std::string> Available;

    public:
        void add(const std::string& item)
        {
            available.emplace(item);
        }

        void clear()
        {
            available.clear();
        }

        auto empty() const
        {
            return available.empty();
        }

        bool is_available(const std::string& item) const
        {
            return std::ranges::find(available, item) != available.end();
        }

        auto size() const
        {
            return available.size();
        }

    public:
        auto begin() const
        {
            return available.begin();
        }

        auto end() const
        {
            return available.end();
        }

        auto cbegin() const
        {
            return available.cbegin();
        }

        auto cend() const
        {
            return available.cend();
        }

    public:
        bool operator==(const Configurations& other) const
        {
            return available == other.available;
        }

    private:
        Available available;
    };
}
