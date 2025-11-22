#include "System.hpp"

#include <Psapi.h>
#include <Windows.h>

namespace Gecko
{
    float System::get_cpu_usage()
    {
        static FILETIME m_ftPrevSysKernel { 0, 0 };
        static FILETIME m_ftPrevSysUser { 0, 0 };

        static FILETIME m_ftPrevProcKernel { 0, 0 };
        static FILETIME m_ftPrevProcUser { 0, 0 };

        FILETIME ftSysIdle { 0, 0 };
        FILETIME ftSysKernel { 0, 0 };
        FILETIME ftSysUser { 0, 0 };

        FILETIME ftProcCreation { 0, 0 };
        FILETIME ftProcExit { 0, 0 };
        FILETIME ftProcKernel { 0, 0 };
        FILETIME ftProcUser { 0, 0 };

        GetSystemTimes(&ftSysIdle, &ftSysKernel, &ftSysUser);
        GetProcessTimes(GetCurrentProcess(), &ftProcCreation, &ftProcExit, &ftProcKernel, &ftProcUser);

        auto ftSysKernelDiff = SubtractTimes(ftSysKernel, m_ftPrevSysKernel);
        auto ftSysUserDiff   = SubtractTimes(ftSysUser, m_ftPrevSysUser);

        auto ftProcKernelDiff = SubtractTimes(ftProcKernel, m_ftPrevProcKernel);
        auto ftProcUserDiff   = SubtractTimes(ftProcUser, m_ftPrevProcUser);

        auto nTotalSys  = ftSysKernelDiff  + ftSysUserDiff;
        auto nTotalProc = ftProcKernelDiff + ftProcUserDiff;

        m_ftPrevSysKernel = ftSysKernel;
        m_ftPrevSysUser   = ftSysUser;

        m_ftPrevProcKernel = ftProcKernel;
        m_ftPrevProcUser   = ftProcUser;

        return ((100.0f * static_cast<float>(nTotalProc)) / static_cast<float>(nTotalSys));
    }

    System::memory_t System::get_memory_usage()
    {
        PROCESS_MEMORY_COUNTERS_EX pmc;

        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

        memory_t memory;

        memory.physical_memory = static_cast<std::uint32_t>(pmc.WorkingSetSize);
        memory.virtual_memory = static_cast<std::uint32_t>(pmc.PrivateUsage);

        return memory;
    }

    ULONGLONG System::SubtractTimes(const FILETIME& ftA, const FILETIME& ftB)
    {
        LARGE_INTEGER a, b;

        a.LowPart = ftA.dwLowDateTime;
        a.HighPart = ftA.dwHighDateTime;

        b.LowPart = ftB.dwLowDateTime;
        b.HighPart = ftB.dwHighDateTime;

        return a.QuadPart - b.QuadPart;
    }
}
