#pragma once

#include "Gecko/Id.hpp"
#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class LogWidget :
        public Widget
    {
	// From Widget.
    public:
        void init_data_bindigs(Rml::Context* context) override;

    // LogWidget.
    public:
        void write(const std::string& message, const std::string& type, const Id& id = Id::Empty);

    private:
        struct Entry
        {
            std::string type;
            std::string message;
        };

        Rml::Vector<Entry> m_entries;
    };
}
