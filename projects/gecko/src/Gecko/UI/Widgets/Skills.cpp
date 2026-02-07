#include "Gecko/UI/Widgets/Skills.hpp"

namespace Gecko
{
    void SkillsWidget::ProcessEvent(Rml::Event& event)
    {
    }

    void SkillsWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("skills"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Skill>())
            {
                handle.RegisterMember("name", &Skill::name);

                constructor.RegisterArray<Rml::Vector<Skill>>();
                constructor.Bind("skills", &m_skills);
                constructor.Bind("selected_skill", &m_selected_skill);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void SkillsWidget::init_events(Rml::ElementDocument* document)
    {
    }

    void SkillsWidget::deinit_events(Rml::ElementDocument* document)
    {
    }

    void SkillsWidget::update(const std::set<std::string>& skills)
    {
        m_skills.clear();

        for (const auto& i : skills)
        {
            m_skills.push_back({ i });
        }

        m_model.DirtyVariable("skills");
    }
}
