#pragma once

#include <Windows.h>

// Select NVIDIA card.
// https://docs.nvidia.com/gameworks/content/technologies/desktop/optimus.htm
extern "C"
{
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
