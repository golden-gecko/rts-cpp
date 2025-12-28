#pragma once

#include "Gecko/Exception.hpp"

namespace Gecko::Utils::Random
{
    static std::random_device device;
    static std::default_random_engine engine(device());

    template<typename Type>
    Type get_int(Type min, Type max)
    {
        std::uniform_int_distribution<Type> distribution(min, max);

        return distribution(engine);
    }

    Ogre::Vector3 get_position(const Ogre::Vector3& min, const Ogre::Vector3& max);

    template<typename Type>
    Type get_real(Type min, Type max)
    {
        if (min > max)
        {
            throw Exception("'min' is greater than 'max'.");
        }

        std::uniform_real_distribution<Type> distribution(min, max);

        return distribution(engine);
    }
}
