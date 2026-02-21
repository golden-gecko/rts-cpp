#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class ConfigurationsWidget :
        public Widget
    {
    public:
        struct Configuration
        {
            std::string name;
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
        void select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void select(const std::string configuration);
        void update(const std::set<std::string>& configurations);

    public:
        const std::string& get_selected() const
        {
            return m_selected_configuration;
        }

    private:
        Rml::Vector<Configuration> m_configurations;
        std::string m_selected_configuration;
    };
}
