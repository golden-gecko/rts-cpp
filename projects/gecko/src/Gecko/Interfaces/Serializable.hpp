#pragma once

namespace Gecko
{
    class Serializable
    {
    public:
        virtual ~Serializable() = default;

        virtual ConfigurationPtr serialize() const = 0;
        virtual void deserialize(const ConfigurationPtr& configuration) = 0;
    };
}
