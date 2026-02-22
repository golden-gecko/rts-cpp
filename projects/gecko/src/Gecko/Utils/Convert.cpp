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

    Id to_id(Ogre::MovableObject* movable_object)
    {
        if (movable_object == nullptr)
        {
            return Id::Empty;
        }

        const Ogre::Any& user_any = movable_object->getUserObjectBindings().getUserAny();

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

    Layer* to_layer(Ogre::MovableObject* movable_object)
    {
        if (movable_object == nullptr)
        {
            return nullptr;
        }

        const Ogre::Any& user_any = movable_object->getUserObjectBindings().getUserAny();

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

    Ogre::PolygonMode to_polygon_mode(const std::string& polygon_mode)
    {
        static std::map<std::string, Ogre::PolygonMode> modes =
        {
            { "points"   , Ogre::PolygonMode::PM_POINTS    },
            { "wireframe", Ogre::PolygonMode::PM_WIREFRAME },
            { "solid"    , Ogre::PolygonMode::PM_SOLID     }
        };

        auto mode = modes.find(polygon_mode);

        if (mode != modes.end())
        {
            return mode->second;
        }

        return Ogre::PolygonMode::PM_SOLID;
    }

    Ogre::ProjectionType to_projection_type(const std::string& projection_type)
    {
        static std::map<std::string, Ogre::ProjectionType> types =
        {
            { "orthographic", Ogre::ProjectionType::PT_ORTHOGRAPHIC },
            { "perspective" , Ogre::ProjectionType::PT_PERSPECTIVE  }
        };

        auto type = types.find(projection_type);

        if (type != types.end())
        {
            return type->second;
        }

        return Ogre::ProjectionType::PT_PERSPECTIVE;
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

        return 0;
    }

    Rml::Input::KeyIdentifier Convert::to_rmlui_key(OIS::KeyCode key)
    {
        switch (key)
        {
            case OIS::KeyCode::KC_A: return Rml::Input::KeyIdentifier::KI_A;
            case OIS::KeyCode::KC_B: return Rml::Input::KeyIdentifier::KI_B;
            case OIS::KeyCode::KC_C: return Rml::Input::KeyIdentifier::KI_C;
            case OIS::KeyCode::KC_D: return Rml::Input::KeyIdentifier::KI_D;
            case OIS::KeyCode::KC_E: return Rml::Input::KeyIdentifier::KI_E;
            case OIS::KeyCode::KC_F: return Rml::Input::KeyIdentifier::KI_F;
            case OIS::KeyCode::KC_G: return Rml::Input::KeyIdentifier::KI_G;
            case OIS::KeyCode::KC_H: return Rml::Input::KeyIdentifier::KI_H;
            case OIS::KeyCode::KC_I: return Rml::Input::KeyIdentifier::KI_I;
            case OIS::KeyCode::KC_J: return Rml::Input::KeyIdentifier::KI_J;
            case OIS::KeyCode::KC_K: return Rml::Input::KeyIdentifier::KI_K;
            case OIS::KeyCode::KC_L: return Rml::Input::KeyIdentifier::KI_L;
            case OIS::KeyCode::KC_M: return Rml::Input::KeyIdentifier::KI_M;
            case OIS::KeyCode::KC_N: return Rml::Input::KeyIdentifier::KI_N;
            case OIS::KeyCode::KC_O: return Rml::Input::KeyIdentifier::KI_O;
            case OIS::KeyCode::KC_P: return Rml::Input::KeyIdentifier::KI_P;
            case OIS::KeyCode::KC_Q: return Rml::Input::KeyIdentifier::KI_Q;
            case OIS::KeyCode::KC_R: return Rml::Input::KeyIdentifier::KI_R;
            case OIS::KeyCode::KC_S: return Rml::Input::KeyIdentifier::KI_S;
            case OIS::KeyCode::KC_T: return Rml::Input::KeyIdentifier::KI_T;
            case OIS::KeyCode::KC_U: return Rml::Input::KeyIdentifier::KI_U;
            case OIS::KeyCode::KC_V: return Rml::Input::KeyIdentifier::KI_V;
            case OIS::KeyCode::KC_W: return Rml::Input::KeyIdentifier::KI_W;
            case OIS::KeyCode::KC_X: return Rml::Input::KeyIdentifier::KI_X;
            case OIS::KeyCode::KC_Y: return Rml::Input::KeyIdentifier::KI_Y;
            case OIS::KeyCode::KC_Z: return Rml::Input::KeyIdentifier::KI_Z;

            case OIS::KeyCode::KC_1: return Rml::Input::KeyIdentifier::KI_0;
            case OIS::KeyCode::KC_2: return Rml::Input::KeyIdentifier::KI_1;
            case OIS::KeyCode::KC_3: return Rml::Input::KeyIdentifier::KI_2;
            case OIS::KeyCode::KC_4: return Rml::Input::KeyIdentifier::KI_3;
            case OIS::KeyCode::KC_5: return Rml::Input::KeyIdentifier::KI_4;
            case OIS::KeyCode::KC_6: return Rml::Input::KeyIdentifier::KI_5;
            case OIS::KeyCode::KC_7: return Rml::Input::KeyIdentifier::KI_6;
            case OIS::KeyCode::KC_8: return Rml::Input::KeyIdentifier::KI_7;
            case OIS::KeyCode::KC_9: return Rml::Input::KeyIdentifier::KI_8;
            case OIS::KeyCode::KC_0: return Rml::Input::KeyIdentifier::KI_9;

	        case OIS::KeyCode::KC_BACK: return Rml::Input::KI_BACK;
	        case OIS::KeyCode::KC_TAB: return Rml::Input::KI_TAB;
	        case OIS::KeyCode::KC_RETURN: return Rml::Input::KI_RETURN;
	        case OIS::KeyCode::KC_PAUSE: return Rml::Input::KI_PAUSE;
	        case OIS::KeyCode::KC_CAPITAL: return Rml::Input::KI_CAPITAL;
	        case OIS::KeyCode::KC_ESCAPE: return Rml::Input::KI_ESCAPE;
	        case OIS::KeyCode::KC_SPACE: return Rml::Input::KI_SPACE;
	        case OIS::KeyCode::KC_END: return Rml::Input::KI_END;
	        case OIS::KeyCode::KC_HOME: return Rml::Input::KI_HOME;
	        case OIS::KeyCode::KC_LEFT: return Rml::Input::KI_LEFT;
	        case OIS::KeyCode::KC_UP: return Rml::Input::KI_UP;
	        case OIS::KeyCode::KC_RIGHT: return Rml::Input::KI_RIGHT;
	        case OIS::KeyCode::KC_DOWN: return Rml::Input::KI_DOWN;
	        case OIS::KeyCode::KC_INSERT: return Rml::Input::KI_INSERT;
	        case OIS::KeyCode::KC_DELETE: return Rml::Input::KI_DELETE;
	        case OIS::KeyCode::KC_LWIN: return Rml::Input::KI_LWIN;
	        case OIS::KeyCode::KC_RWIN: return Rml::Input::KI_RWIN;

	        case OIS::KeyCode::KC_NUMPAD0: return Rml::Input::KI_NUMPAD0;
	        case OIS::KeyCode::KC_NUMPAD1: return Rml::Input::KI_NUMPAD1;
	        case OIS::KeyCode::KC_NUMPAD2: return Rml::Input::KI_NUMPAD2;
	        case OIS::KeyCode::KC_NUMPAD3: return Rml::Input::KI_NUMPAD3;
	        case OIS::KeyCode::KC_NUMPAD4: return Rml::Input::KI_NUMPAD4;
	        case OIS::KeyCode::KC_NUMPAD5: return Rml::Input::KI_NUMPAD5;
	        case OIS::KeyCode::KC_NUMPAD6: return Rml::Input::KI_NUMPAD6;
	        case OIS::KeyCode::KC_NUMPAD7: return Rml::Input::KI_NUMPAD7;
	        case OIS::KeyCode::KC_NUMPAD8: return Rml::Input::KI_NUMPAD8;
	        case OIS::KeyCode::KC_NUMPAD9: return Rml::Input::KI_NUMPAD9;
	        case OIS::KeyCode::KC_NUMPADENTER: return Rml::Input::KI_NUMPADENTER;
	        case OIS::KeyCode::KC_MULTIPLY: return Rml::Input::KI_MULTIPLY;
	        case OIS::KeyCode::KC_ADD: return Rml::Input::KI_ADD;
	        case OIS::KeyCode::KC_SUBTRACT: return Rml::Input::KI_SUBTRACT;
	        case OIS::KeyCode::KC_DECIMAL: return Rml::Input::KI_DECIMAL;
	        case OIS::KeyCode::KC_DIVIDE: return Rml::Input::KI_DIVIDE;

            case OIS::KeyCode::KC_F1: return Rml::Input::KI_F1;
            case OIS::KeyCode::KC_F2: return Rml::Input::KI_F2;
            case OIS::KeyCode::KC_F3: return Rml::Input::KI_F3;
            case OIS::KeyCode::KC_F4: return Rml::Input::KI_F4;
            case OIS::KeyCode::KC_F5: return Rml::Input::KI_F5;
            case OIS::KeyCode::KC_F6: return Rml::Input::KI_F6;
            case OIS::KeyCode::KC_F7: return Rml::Input::KI_F7;
            case OIS::KeyCode::KC_F8: return Rml::Input::KI_F8;
            case OIS::KeyCode::KC_F9: return Rml::Input::KI_F9;
            case OIS::KeyCode::KC_F10: return Rml::Input::KI_F10;
            case OIS::KeyCode::KC_F11: return Rml::Input::KI_F11;
            case OIS::KeyCode::KC_F12: return Rml::Input::KI_F12;
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

    std::string to_string(const Json::Value& value, const std::string& indentation)
    {
        Json::StreamWriterBuilder builder;

        builder["commentStyle"] = "None";
        builder["indentation"] = indentation;

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
}
