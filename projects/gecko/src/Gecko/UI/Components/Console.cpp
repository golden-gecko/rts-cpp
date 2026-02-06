#include "Gecko/UI/Components/Console.hpp"

#include "Gecko/Log.hpp"

namespace Gecko
{
    void Console::ProcessEvent(Rml::Event& event)
    {
        L_TRACE << "Console::ProcessEvent()";

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
                        m_current_command = m_commands.size() - 1;

                        event.GetCurrentElement()->SetAttribute<std::string>("value", "");
                    }

                    event.StopPropagation();

                    break;
                }

                case Rml::Input::KeyIdentifier::KI_UP:
                {
                    if (m_current_command >= 0 && m_current_command < m_commands.size())
                    {
                        event.GetCurrentElement()->SetAttribute<std::string>("value", m_commands[m_current_command].command);

                        if (m_current_command > 0)
                        {
                            m_current_command -= 1;
                        }
                    }

                    event.StopPropagation();

                    break;
                }

                case Rml::Input::KeyIdentifier::KI_DOWN:
                {
                    if (m_current_command >= 0 && m_current_command < m_commands.size())
                    {
                        event.GetCurrentElement()->SetAttribute<std::string>("value", m_commands[m_current_command].command);

                        if (m_current_command < m_commands.size() - 1)
                        {
                            m_current_command += 1;
                        }
                    }

                    event.StopPropagation();

                    break;
                }
            }
        }
    }

    void Console::init_events(Rml::Element* element)
    {
        element->GetElementById("console-input")->AddEventListener(Rml::EventId::Keydown, this);
    }

    void Console::deinit_events(Rml::Element* element)
    {
        element->GetElementById("console-input")->RemoveEventListener(Rml::EventId::Keydown, this);
    }
}
