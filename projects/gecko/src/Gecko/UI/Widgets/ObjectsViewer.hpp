#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class ObjectsViewerWidget :
        public Widget
    {
	// From Widget.
	public:
        void init_data_bindigs(Rml::Context* context) override;

    // ObjectsViewerWidget.
    public:
        void close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        struct Object
        {
            int         id;
            std::string name;
            std::size_t order_count;
            std::string current_order_name;
        };

        Rml::Vector<Object> m_objects;
    };
}
