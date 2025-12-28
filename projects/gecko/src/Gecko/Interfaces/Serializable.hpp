#pragma once

namespace Gecko
{
    class Serializable
    {
    public:
        virtual ~Serializable() = default;

        virtual std::shared_ptr<Configuration> serialize() const = 0;
        virtual void deserialize(const std::shared_ptr<Configuration>& configuration) = 0;
    };
}
