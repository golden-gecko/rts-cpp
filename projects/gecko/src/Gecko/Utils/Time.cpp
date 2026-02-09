#include "Gecko/Utils/Time.hpp"

#include "Gecko/Log.hpp"

namespace Gecko::Utils::Time
{
    Value get_current()
    {
        return std::chrono::high_resolution_clock::now();
    }

    float get_duration(const Value& start, const Value& end)
    {
        return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1>>>(end - start).count();
    }

    Scope::Scope(const std::string& name) :
        m_name(name) , m_start(get_current())
    {
    }

    Scope::~Scope()
    {
        L_DEBUG << m_name << ": " << get_duration(m_start, get_current());
    }
}
