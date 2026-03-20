#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class InfoWidget :
        public Widget
    {
	// From Widget.
    public:
        void init_data_bindigs(Rml::Context* context) override;

    // InfoWidget.
    public:
        void update(const Id& hovered_object_id);

    public:
        void set_description(const std::string& text);

    private:
        struct Description
        {
            std::string text;
        };

        Description m_description;
    };
}
