#pragma once

namespace Gecko
{
    class System
    {
    public:
        struct memory_t
        {
            std::uint32_t physical_memory;
            std::uint32_t virtual_memory;
        };

        static float get_cpu_usage();
        static memory_t get_memory_usage();

    private:
        static ULONGLONG SubtractTimes(const FILETIME& ftA, const FILETIME& ftB);
    };
}
