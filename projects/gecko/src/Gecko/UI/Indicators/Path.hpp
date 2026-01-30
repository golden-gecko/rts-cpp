#pragma once

#include "Gecko/Navigation/Path.hpp"
#include "Gecko/UI/Indicators/Indicator.hpp"

namespace Gecko
{
    class Path :
        public Indicator
    {
    private:
        using base_type = Indicator;

    public:
        explicit Path();

        ~Path() override;

    public:
        void set_points(const std::string& material_name, const Navigation::Path::Points& points);

    private:
        Ogre::ManualObject* m_manual_object;
    };
}
