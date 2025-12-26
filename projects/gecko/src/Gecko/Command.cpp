#include "Gecko/Command.hpp"

#include "Gecko/Exception.hpp"
#include "Gecko/Utils/Utils.hpp"

namespace Gecko::Command
{
    /*
    static const boost::bimap<Value, std::string> type_to_string = boost::assign::list_of<boost::bimap<Value, std::string>::relation>
        TYPE_TO_STRING(Value, Assign_To_Group_0)
        TYPE_TO_STRING(Value, Assign_To_Group_1)
        TYPE_TO_STRING(Value, Assign_To_Group_2)
        TYPE_TO_STRING(Value, Assign_To_Group_3)
        TYPE_TO_STRING(Value, Assign_To_Group_4)
        TYPE_TO_STRING(Value, Assign_To_Group_5)
        TYPE_TO_STRING(Value, Assign_To_Group_6)
        TYPE_TO_STRING(Value, Assign_To_Group_7)
        TYPE_TO_STRING(Value, Assign_To_Group_8)
        TYPE_TO_STRING(Value, Assign_To_Group_9)

        TYPE_TO_STRING(Value, Camera_Move_Backward)
        TYPE_TO_STRING(Value, Camera_Move_Down)
        TYPE_TO_STRING(Value, Camera_Move_Forward)
        TYPE_TO_STRING(Value, Camera_Move_Left)
        TYPE_TO_STRING(Value, Camera_Move_Right)
        TYPE_TO_STRING(Value, Camera_Move_Up)

        TYPE_TO_STRING(Value, Game_Faster)
        TYPE_TO_STRING(Value, Game_Pause)
        TYPE_TO_STRING(Value, Game_Slower)

        TYPE_TO_STRING(Value, Multiple_Order)
        TYPE_TO_STRING(Value, Multiple_Select)

        TYPE_TO_STRING(Value, Order_Attack)
        TYPE_TO_STRING(Value, Order_Create)
        TYPE_TO_STRING(Value, Order_Destroy)
        TYPE_TO_STRING(Value, Order_Disable)
        TYPE_TO_STRING(Value, Order_Enable)
        TYPE_TO_STRING(Value, Order_Follow)
        TYPE_TO_STRING(Value, Order_Guard)
        TYPE_TO_STRING(Value, Order_Load)
        TYPE_TO_STRING(Value, Order_Move)
        TYPE_TO_STRING(Value, Order_Patrol)
        TYPE_TO_STRING(Value, Order_Rally)
        TYPE_TO_STRING(Value, Order_Stop)
        TYPE_TO_STRING(Value, Order_Unload)
        TYPE_TO_STRING(Value, Order_Wait)

        TYPE_TO_STRING(Value, Select_Group_0)
        TYPE_TO_STRING(Value, Select_Group_1)
        TYPE_TO_STRING(Value, Select_Group_2)
        TYPE_TO_STRING(Value, Select_Group_3)
        TYPE_TO_STRING(Value, Select_Group_4)
        TYPE_TO_STRING(Value, Select_Group_5)
        TYPE_TO_STRING(Value, Select_Group_6)
        TYPE_TO_STRING(Value, Select_Group_7)
        TYPE_TO_STRING(Value, Select_Group_8)
        TYPE_TO_STRING(Value, Select_Group_9)

        TYPE_TO_STRING(Value, Switch_Player_1)
        TYPE_TO_STRING(Value, Switch_Player_2)
        TYPE_TO_STRING(Value, Switch_Player_3)
        TYPE_TO_STRING(Value, Switch_Player_4)
        TYPE_TO_STRING(Value, Switch_Player_5)
        TYPE_TO_STRING(Value, Switch_Player_6)
        TYPE_TO_STRING(Value, Switch_Player_7)
        TYPE_TO_STRING(Value, Switch_Player_8)

        TYPE_TO_STRING(Value, UI_Show_Menu)
        TYPE_TO_STRING(Value, UI_Toggle_Floating_Description)

        TYPE_TO_STRING(Value, Quit)
    ;
    */

    std::string to_string(Value value)
    {
        /*
        auto i = type_to_string.left.find(value);

        if (i == type_to_string.left.end())
        {
            throw Exception("Value not found.");
        }

        return i->second;
        */

        return "";
    }

    Value from_string(const std::string& value)
    {
        /*
        auto i = type_to_string.right.find(value);

        if (i == type_to_string.right.end())
        {
            throw Exception("Value '" + value + "' not found.");
        }

        return i->second;
        */

        return Value();
    }
}
