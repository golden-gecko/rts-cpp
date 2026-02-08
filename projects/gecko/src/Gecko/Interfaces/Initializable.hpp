#pragma once

namespace Gecko
{
    class Initializable
    {
    public:
        virtual ~Initializable() = default;
        
    public:
        virtual void init() = 0;
        virtual void deinit() = 0;
    };
}
