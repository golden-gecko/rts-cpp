#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class ResourcesWidget :
        public Widget
    {
    // From Widget.
	public:
        void init_data_bindigs(Rml::Context* context) override;

    // ResourcesWidget.
    public:
        void update(const ResourcesPtr& resources);

    private:
        struct Resource
        {
            std::string   name;
            std::uint64_t current;
            std::uint64_t max;
            std::string   direction;
            float         ratio;
        };

        Rml::Vector<Resource> m_resources;
    };
}
