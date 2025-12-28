#include "Gecko/Utils/Time.hpp"

#include "Gecko/Log.hpp"

namespace Gecko::Utils::Time
{
    Value get()
    {
        return std::chrono::high_resolution_clock::now();
    }

    float get_duration(const Value& start, const Value& end)
    {
        return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1>>>(end - start).count();
    }

    Scope::Scope(const std::string& name) :
        name(name) , start(get())
    {
    }

    Scope::~Scope()
    {
        L_DEBUG << name << ": " << get_duration(start, get());
    }
}
