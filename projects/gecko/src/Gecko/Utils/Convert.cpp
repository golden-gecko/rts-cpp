#include "Gecko/Utils/Convert.hpp"

namespace Gecko::Utils::Convert
{
    const std::map<OIS::KeyCode, char> lower = boost::assign::list_of<std::map<OIS::KeyCode, char>::value_type>
        (OIS::KeyCode::KC_1, '1')
        (OIS::KeyCode::KC_2, '2')
        (OIS::KeyCode::KC_3, '3')
        (OIS::KeyCode::KC_4, '4')
        (OIS::KeyCode::KC_5, '5')
        (OIS::KeyCode::KC_6, '6')
        (OIS::KeyCode::KC_7, '7')
        (OIS::KeyCode::KC_8, '8')
        (OIS::KeyCode::KC_9, '9')
        (OIS::KeyCode::KC_0, '0')
        (OIS::KeyCode::KC_MINUS, '-')
        (OIS::KeyCode::KC_EQUALS, '=')
        (OIS::KeyCode::KC_Q, 'q')
        (OIS::KeyCode::KC_W, 'w')
        (OIS::KeyCode::KC_E, 'e')
        (OIS::KeyCode::KC_R, 'r')
        (OIS::KeyCode::KC_T, 't')
        (OIS::KeyCode::KC_Y, 'y')
        (OIS::KeyCode::KC_U, 'u')
        (OIS::KeyCode::KC_I, 'i')
        (OIS::KeyCode::KC_O, 'o')
        (OIS::KeyCode::KC_P, 'p')
        (OIS::KeyCode::KC_LBRACKET, '[')
        (OIS::KeyCode::KC_RBRACKET, ']')
        (OIS::KeyCode::KC_A, 'a')
        (OIS::KeyCode::KC_S, 's')
        (OIS::KeyCode::KC_D, 'd')
        (OIS::KeyCode::KC_F, 'f')
        (OIS::KeyCode::KC_G, 'g')
        (OIS::KeyCode::KC_H, 'h')
        (OIS::KeyCode::KC_J, 'j')
        (OIS::KeyCode::KC_K, 'k')
        (OIS::KeyCode::KC_L, 'l')
        (OIS::KeyCode::KC_SEMICOLON, ';')
        (OIS::KeyCode::KC_APOSTROPHE, '\'')
        (OIS::KeyCode::KC_BACKSLASH, '\\')
        (OIS::KeyCode::KC_Z, 'z')
        (OIS::KeyCode::KC_X, 'x')
        (OIS::KeyCode::KC_C, 'c')
        (OIS::KeyCode::KC_V, 'v')
        (OIS::KeyCode::KC_B, 'b')
        (OIS::KeyCode::KC_N, 'n')
        (OIS::KeyCode::KC_M, 'm')
        (OIS::KeyCode::KC_COMMA, ',')
        (OIS::KeyCode::KC_PERIOD, '.')
        (OIS::KeyCode::KC_SLASH, '/')
        (OIS::KeyCode::KC_MULTIPLY, '*')
        (OIS::KeyCode::KC_SPACE, ' ')
        (OIS::KeyCode::KC_NUMPAD7, '7')
        (OIS::KeyCode::KC_NUMPAD8, '8')
        (OIS::KeyCode::KC_NUMPAD9, '9')
        (OIS::KeyCode::KC_SUBTRACT, '-')
        (OIS::KeyCode::KC_NUMPAD4, '4')
        (OIS::KeyCode::KC_NUMPAD5, '5')
        (OIS::KeyCode::KC_NUMPAD6, '6')
        (OIS::KeyCode::KC_ADD, '+')
        (OIS::KeyCode::KC_NUMPAD1, '1')
        (OIS::KeyCode::KC_NUMPAD2, '2')
        (OIS::KeyCode::KC_NUMPAD3, '3')
        (OIS::KeyCode::KC_NUMPAD0, '0')
        (OIS::KeyCode::KC_NUMPADCOMMA, ',')
        (OIS::KeyCode::KC_DIVIDE, '/')
    ;

    const std::map<OIS::KeyCode, char> upper = boost::assign::list_of<std::map<OIS::KeyCode, char>::value_type>
        (OIS::KeyCode::KC_1, '1')
        (OIS::KeyCode::KC_2, '2')
        (OIS::KeyCode::KC_3, '3')
        (OIS::KeyCode::KC_4, '4')
        (OIS::KeyCode::KC_5, '5')
        (OIS::KeyCode::KC_6, '6')
        (OIS::KeyCode::KC_7, '7')
        (OIS::KeyCode::KC_8, '8')
        (OIS::KeyCode::KC_9, '9')
        (OIS::KeyCode::KC_0, '0')
        (OIS::KeyCode::KC_MINUS, '_')
        (OIS::KeyCode::KC_EQUALS, '+')
        (OIS::KeyCode::KC_Q, 'Q')
        (OIS::KeyCode::KC_W, 'W')
        (OIS::KeyCode::KC_E, 'E')
        (OIS::KeyCode::KC_R, 'R')
        (OIS::KeyCode::KC_T, 'T')
        (OIS::KeyCode::KC_Y, 'Y')
        (OIS::KeyCode::KC_U, 'U')
        (OIS::KeyCode::KC_I, 'I')
        (OIS::KeyCode::KC_O, 'O')
        (OIS::KeyCode::KC_P, 'P')
        (OIS::KeyCode::KC_LBRACKET, '{')
        (OIS::KeyCode::KC_RBRACKET, '}')
        (OIS::KeyCode::KC_A, 'A')
        (OIS::KeyCode::KC_S, 'S')
        (OIS::KeyCode::KC_D, 'D')
        (OIS::KeyCode::KC_F, 'F')
        (OIS::KeyCode::KC_G, 'G')
        (OIS::KeyCode::KC_H, 'H')
        (OIS::KeyCode::KC_J, 'J')
        (OIS::KeyCode::KC_K, 'K')
        (OIS::KeyCode::KC_L, 'L')
        (OIS::KeyCode::KC_SEMICOLON, ':')
        (OIS::KeyCode::KC_APOSTROPHE, '"')
        (OIS::KeyCode::KC_BACKSLASH, '|')
        (OIS::KeyCode::KC_Z, 'Z')
        (OIS::KeyCode::KC_X, 'X')
        (OIS::KeyCode::KC_C, 'C')
        (OIS::KeyCode::KC_V, 'V')
        (OIS::KeyCode::KC_B, 'B')
        (OIS::KeyCode::KC_N, 'N')
        (OIS::KeyCode::KC_M, 'M')
        (OIS::KeyCode::KC_COMMA, '<')
        (OIS::KeyCode::KC_PERIOD, '>')
        (OIS::KeyCode::KC_SLASH, '?')
        (OIS::KeyCode::KC_MULTIPLY, '*')
        (OIS::KeyCode::KC_SPACE, ' ')
        (OIS::KeyCode::KC_NUMPAD7, '7')
        (OIS::KeyCode::KC_NUMPAD8, '8')
        (OIS::KeyCode::KC_NUMPAD9, '9')
        (OIS::KeyCode::KC_SUBTRACT, '-')
        (OIS::KeyCode::KC_NUMPAD4, '4')
        (OIS::KeyCode::KC_NUMPAD5, '5')
        (OIS::KeyCode::KC_NUMPAD6, '6')
        (OIS::KeyCode::KC_ADD, '+')
        (OIS::KeyCode::KC_NUMPAD1, '1')
        (OIS::KeyCode::KC_NUMPAD2, '2')
        (OIS::KeyCode::KC_NUMPAD3, '3')
        (OIS::KeyCode::KC_NUMPAD0, '0')
        (OIS::KeyCode::KC_NUMPADCOMMA, ',')
        (OIS::KeyCode::KC_DIVIDE, '/')
    ;

    char to_char(OIS::KeyCode key_code, bool shift)
    {
        if (shift)
        {
            auto i = lower.find(key_code);

            if (i == lower.end())
            {
                return '\0';
            }

            return i->second;
        }
        else
        {
            auto i = upper.find(key_code);

            if (i == upper.end())
            {
                return '\0';
            }

            return i->second;
        }
    }

    Id to_id(const Ogre::Entity& entity)
    {
        const auto& user_any = entity.getUserObjectBindings().getUserAny();

        if (user_any.has_value() == false)
        {
            return Id::Empty;
        }

        if (user_any.type() != typeid(Id))
        {
            return Id::Empty;
        }

        return Ogre::any_cast<Id>(user_any);
    }

    Id to_id(const Ogre::MovableObject& entity)
    {
        const auto& user_any = entity.getUserObjectBindings().getUserAny();

        if (user_any.has_value() == false)
        {
            return Id::Empty;
        }

        if (user_any.type() != typeid(Id))
        {
            return Id::Empty;
        }

        return Ogre::any_cast<Id>(user_any);
    }

    Layer* to_layer(Ogre::Entity* entity)
    {
        const auto& user_any = entity->getUserObjectBindings().getUserAny();

        if (user_any.isEmpty())
        {
            return nullptr;
        }

        if (user_any.type() != typeid(Layer*))
        {
            return nullptr;
        }

        return Ogre::any_cast<Layer*>(user_any);
    }

    Ogre::RealRect to_rectangle_coordinates(const Ogre::RealRect& rect)
    {
        float left = 2.0f * rect.left - 1.0f;
        float right = 2.0f * rect.right - 1.0f;
        float top = 1.0f - 2.0f * rect.top;
        float bottom = 1.0f - 2.0f * rect.bottom;

        return Ogre::RealRect(left, top, right, bottom);
    }

    int Convert::to_rmlui_button(OIS::MouseButtonID button)
    {
        switch (button)
        {
            case OIS::MouseButtonID::MB_Left  : return 0;
            case OIS::MouseButtonID::MB_Right : return 1;
            case OIS::MouseButtonID::MB_Middle: return 2;
        }

        return OIS::MouseButtonID::MB_Left;
    }

    Rml::Input::KeyIdentifier Convert::to_rmlui_key(char key)
    {
        switch (key)
        {
            case 'a': return Rml::Input::KeyIdentifier::KI_A;
            case 'b': return Rml::Input::KeyIdentifier::KI_B;
            case 'c': return Rml::Input::KeyIdentifier::KI_C;
            case 'd': return Rml::Input::KeyIdentifier::KI_D;
            case 'e': return Rml::Input::KeyIdentifier::KI_E;
            case 'f': return Rml::Input::KeyIdentifier::KI_F;
            case 'g': return Rml::Input::KeyIdentifier::KI_G;
            case 'h': return Rml::Input::KeyIdentifier::KI_H;
            case 'i': return Rml::Input::KeyIdentifier::KI_I;
            case 'j': return Rml::Input::KeyIdentifier::KI_J;
            case 'k': return Rml::Input::KeyIdentifier::KI_K;
            case 'l': return Rml::Input::KeyIdentifier::KI_L;
            case 'm': return Rml::Input::KeyIdentifier::KI_M;
            case 'n': return Rml::Input::KeyIdentifier::KI_N;
            case 'o': return Rml::Input::KeyIdentifier::KI_O;
            case 'p': return Rml::Input::KeyIdentifier::KI_P;
            case 'q': return Rml::Input::KeyIdentifier::KI_Q;
            case 'r': return Rml::Input::KeyIdentifier::KI_R;
            case 's': return Rml::Input::KeyIdentifier::KI_S;
            case 't': return Rml::Input::KeyIdentifier::KI_T;
            case 'u': return Rml::Input::KeyIdentifier::KI_U;
            case 'v': return Rml::Input::KeyIdentifier::KI_V;
            case 'w': return Rml::Input::KeyIdentifier::KI_W;
            case 'x': return Rml::Input::KeyIdentifier::KI_X;
            case 'y': return Rml::Input::KeyIdentifier::KI_Y;
            case 'z': return Rml::Input::KeyIdentifier::KI_Z;

            case '0': return Rml::Input::KeyIdentifier::KI_0;
            case '1': return Rml::Input::KeyIdentifier::KI_1;
            case '2': return Rml::Input::KeyIdentifier::KI_2;
            case '3': return Rml::Input::KeyIdentifier::KI_3;
            case '4': return Rml::Input::KeyIdentifier::KI_4;
            case '5': return Rml::Input::KeyIdentifier::KI_5;
            case '6': return Rml::Input::KeyIdentifier::KI_6;
            case '7': return Rml::Input::KeyIdentifier::KI_7;
            case '8': return Rml::Input::KeyIdentifier::KI_8;
            case '9': return Rml::Input::KeyIdentifier::KI_9;

            case 59: return Rml::Input::KI_F1;
            case 60: return Rml::Input::KI_F2;
            case 61: return Rml::Input::KI_F3;
            case 62: return Rml::Input::KI_F4;
            case 63: return Rml::Input::KI_F5;
            case 64: return Rml::Input::KI_F6;
            case 65: return Rml::Input::KI_F7;
            case 66: return Rml::Input::KI_F8;
            case 67: return Rml::Input::KI_F9;
            case 68: return Rml::Input::KI_F10;
            case 69: return Rml::Input::KI_F11;
            case 70: return Rml::Input::KI_F12;
        }

        return Rml::Input::KeyIdentifier::KI_UNKNOWN;
    }

    Ogre::Vector2 to_screen_coordinates(const OIS::MouseEvent& arg)
    {
        return to_screen_coordinates(arg.state.X.abs, arg.state.Y.abs, arg.state.width, arg.state.height);
    }

    Ogre::Vector2 to_screen_coordinates(std::uint32_t left, std::uint32_t top, std::uint32_t width, std::uint32_t height)
    {
        return Ogre::Vector2(
            static_cast<float>(left) / static_cast<float>(width),
            static_cast<float>(top) / static_cast<float>(height)
        );
    }

    std::string to_string(const Json::Value& value)
    {
        Json::StreamWriterBuilder builder;

        builder["commentStyle"] = "None";
        builder["indentation"] = "";

        return Json::writeString(builder, value);
    }

    std::string to_string(const Ogre::ColourValue& value, std::uint8_t precision)
    {
        std::stringstream stream;

        stream << std::fixed << std::setprecision(precision);
        stream << value.r << ":" << value.g << ":" << value.b << ":" << value.a;

        return stream.str();
    }

    std::string to_string(const Ogre::Vector3& value, std::uint8_t precision)
    {
        std::stringstream stream;

        stream << std::fixed << std::setprecision(precision);
        stream << value.x << ":" << value.y << ":" << value.z;

        return stream.str();
    }

    /*
    Window* to_window(const SDL_Event& event)
    {
        auto sdl_window = SDL_GetWindowFromID(event.window.windowID);
        auto window_data = SDL_GetWindowData(sdl_window, "window");

        return reinterpret_cast<Window*>(window_data);
    }
    */
}
