#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class SkillsWidget :
        public Widget
    {
    public:
        struct Skill
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
        void update(const std::set<std::string>& skills);
        
    private:
        Rml::Vector<Skill> m_skills;
        std::string m_selected_skill;
    };
}
