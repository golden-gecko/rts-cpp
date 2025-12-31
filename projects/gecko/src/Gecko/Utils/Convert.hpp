#pragma once

#include "Gecko/Id.hpp"

namespace Gecko::Utils::Convert
{
    char to_char(OIS::KeyCode key_code, bool shift);

    Id to_id(const Ogre::Entity& entity);
    Id to_id(const Ogre::MovableObject& entity);

    Layer* to_layer(Ogre::Entity* entity);

    Ogre::RealRect to_rectangle_coordinates(const Ogre::RealRect& rect);

    int to_rmlui_button(OIS::MouseButtonID button);
    Rml::Input::KeyIdentifier to_rmlui_key(char key);

    Ogre::Vector2 to_screen_coordinates(const OIS::MouseEvent& arg);
    Ogre::Vector2 to_screen_coordinates(std::uint32_t left, std::uint32_t top, std::uint32_t width, std::uint32_t height);

    std::string to_string(const Json::Value& value);
    std::string to_string(const Ogre::ColourValue& value, std::uint8_t precision = 1);
    std::string to_string(const Ogre::Vector3& value, std::uint8_t precision = 1);

    template<typename Type>
    std::string to_string(const Type& value, std::uint8_t precision = 1)
    {
        std::stringstream stream;

        stream << std::fixed << std::setprecision(precision);
        stream << value;

        return stream.str();
    }

    // Window* to_window(const SDL_Event& event);
}
