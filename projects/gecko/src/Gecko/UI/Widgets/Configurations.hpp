#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class ConfigurationsWidget :
        public Widget
    {
	// From Widget.
    public:
        void init_data_bindigs(Rml::Context* context) override;

    // ConfigurationsWidget.
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
        struct Configuration
        {
            std::string name;
        };

        Rml::Vector<Configuration> m_configurations;
        std::string                m_selected_configuration;
    };
}
