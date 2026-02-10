#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class ObjectsViewerWidget :
        public Widget
    {
    public:
        struct Object
        {
            int         id;
            std::string name;
            std::size_t order_count;
            std::string current_order_name;
        };

	public:
		// From Rml::EventListener.
		void ProcessEvent(Rml::Event& event) override;

	public:
		// From Widget.
        void init_data_bindigs(Rml::Context* context) override;

        void init_events(Rml::ElementDocument* document) override;
        void deinit_events(Rml::ElementDocument* document) override;

    public:
        void close(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void update();

    private:
        Rml::ElementDocument* m_document = nullptr;

        Rml::Vector<Object> m_objects;
    };
}
