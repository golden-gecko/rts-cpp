#pragma once

#include "Gecko/UI/Widgets/Widget.hpp"

namespace Gecko
{
    class SkillsWidget :
        public Widget
    {
	// From Widget.
	public:
        void init_data_bindigs(Rml::Context* context) override;

    // SkillsWidget.
    public:
        void select(Rml::DataModelHandle model, Rml::Event& ev, const Rml::VariantList& arguments);
        void select(const std::string configuration);
        void update(const std::set<std::string>& skills);

    public:
        const std::string& get_selected() const
        {
            return m_selected_skill;
        }
        
    private:
        struct Skill
        {
            std::string name;
        };

        Rml::Vector<Skill> m_skills;
        std::string m_selected_skill;
    };
}
