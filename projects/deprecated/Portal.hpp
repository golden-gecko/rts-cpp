#pragma once

#include "Object.hpp"

namespace Gecko
{
    class Portal : public Object
    {
    private:
        using base_type = Object;

    public:
        static Portal* create(Portal* memory, std::shared_ptr<Configuration> configuration);

    public:
        Portal();
        Portal(const Portal& other);

        void init() override;
        void deinit() override;

        void set_position(const Ogre::Vector3& position);
    };
}
