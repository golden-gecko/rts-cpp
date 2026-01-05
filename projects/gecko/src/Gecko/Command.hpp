#pragma once

namespace Gecko::Command
{
    enum class Value :
        std::uint8_t
    {
        Assign_To_Group_0,
        Assign_To_Group_1,
        Assign_To_Group_2,
        Assign_To_Group_3,
        Assign_To_Group_4,
        Assign_To_Group_5,
        Assign_To_Group_6,
        Assign_To_Group_7,
        Assign_To_Group_8,
        Assign_To_Group_9,

        Camera_Move_Backward,
        Camera_Move_Down,
        Camera_Move_Forward,
        Camera_Move_Left,
        Camera_Move_Right,
        Camera_Move_Up,

        Game_Faster,
        Game_Pause,
        Game_Slower,

        Multiple_Order,
        Multiple_Select,

        Order_Attack,
        Order_Create,
        Order_Destroy,
        Order_Disable,
        Order_Enable,
        Order_Follow,
        Order_Guard,
        Order_Load,
        Order_Move,
        Order_Patrol,
        Order_Rally,
        Order_Stop,
        Order_Unload,
        Order_Wait,

        Select_Group_0,
        Select_Group_1,
        Select_Group_2,
        Select_Group_3,
        Select_Group_4,
        Select_Group_5,
        Select_Group_6,
        Select_Group_7,
        Select_Group_8,
        Select_Group_9,

        Switch_Player_1,
        Switch_Player_2,
        Switch_Player_3,
        Switch_Player_4,
        Switch_Player_5,
        Switch_Player_6,
        Switch_Player_7,
        Switch_Player_8,

        UI_Show_Menu,
        UI_Toggle_Floating_Description,

        Quit
    };

    std::string to_string(Value value);

    Value from_string(const std::string& value);
}
