#include "Gecko/UI/Widgets/Console.hpp"

namespace Gecko
{
    void ConsoleWidget::ProcessEvent(Rml::Event& event)
    {
        if (event.GetId() == Rml::EventId::Keydown)
        {
            Rml::Input::KeyIdentifier key_identifier = static_cast<Rml::Input::KeyIdentifier>(event.GetParameter<int>("key_identifier", 0));

            switch (key_identifier)
            {
                case Rml::Input::KeyIdentifier::KI_RETURN:
                {
                    std::string command = event.GetCurrentElement()->GetAttribute("value")->Get<std::string>();

                    if (command.size() >= 0)
                    {
                        time_t t = std::time(nullptr);
                        tm* tm = std::localtime(&t);
                        
                        std::stringstream time;
                        time << std::put_time(tm, "%Y-%m-%d %H-%M-%S");

                        m_commands.push_back({ time.str(), command});
                        m_selected_command = m_commands.size() - 1;

                        event.GetCurrentElement()->SetAttribute<std::string>("value", "");

                        m_model.DirtyVariable("commands");
                    }

                    event.StopPropagation();

                    break;
                }

                case Rml::Input::KeyIdentifier::KI_UP:
                {
                    if (m_selected_command >= 0 && m_selected_command < m_commands.size())
                    {
                        event.GetCurrentElement()->SetAttribute<std::string>("value", m_commands[m_selected_command].command);

                        if (m_selected_command > 0)
                        {
                            m_selected_command -= 1;
                        }

                        m_model.DirtyVariable("commands");
                    }

                    event.StopPropagation();

                    break;
                }

                case Rml::Input::KeyIdentifier::KI_DOWN:
                {
                    if (m_selected_command >= 0 && m_selected_command < m_commands.size())
                    {
                        event.GetCurrentElement()->SetAttribute<std::string>("value", m_commands[m_selected_command].command);

                        if (m_selected_command < m_commands.size() - 1)
                        {
                            m_selected_command += 1;
                        }

                        m_model.DirtyVariable("commands");
                    }

                    event.StopPropagation();

                    break;
                }
            }
        }
    }

    void ConsoleWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("console"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Command>())
            {
                handle.RegisterMember("timestamp", &Command::timestamp);
                handle.RegisterMember("command", &Command::command);

                constructor.RegisterArray<Rml::Vector<Command>>();
                constructor.Bind("commands", &m_commands);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void ConsoleWidget::init_events(Rml::ElementDocument* document)
    {
        base_type::init_events(document);

        if (document)
        {
            if (Rml::Element* element = document->GetElementById("console-input"))
            {
                element->AddEventListener(Rml::EventId::Keydown, this);
            }
        }
    }

    void ConsoleWidget::deinit_events(Rml::ElementDocument* document)
    {
        base_type::deinit_events(document);

        if (document)
        {
            if (Rml::Element* element = document->GetElementById("console-input"))
            {
                element->RemoveEventListener(Rml::EventId::Keydown, this);
            }
        }
    }
}
