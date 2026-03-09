#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class ResourcesWidget :
        public Widget
    {
    public:
        struct Resource
        {
            std::string   name;
            std::uint64_t current;
            std::uint64_t max;
            std::string   direction;
            float         ratio;
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
        void update(const std::shared_ptr<Resources>& resources);

    private:
        Rml::Vector<Resource> m_resources;
    };
}
