#include "Gecko/UI/Events.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/UI/UI.hpp"

namespace Gecko
{
    Rml::EventListener* EventInstancer::InstanceEventListener(const Rml::String& value, Rml::Element * element)
    {
        return new Event(value);
    }

	void Event::ProcessEvent(Rml::Event& event)
	{
		L_TRACE << "Event::ProcessEvent(): value=" << value;

		std::vector<std::string> segments;

        boost::algorithm::split(segments, value, boost::is_any_of(":"), boost::token_compress_on);

		if (segments.size() != 2)
		{
			return;
		}

		if (segments[0] == "call")
		{
			if (segments[1] == "quit-to-menu")
			{
				// TODO: Implement.
			}
			else if (segments[1] == "quit-to-desktop")
			{
				Game::getSingleton().quit();
			}
		}
		else if (segments[0] == "open")
		{
			Rml::ElementDocument* document = UI::getSingleton().get_document();

			if (document == nullptr)
			{
				return;
			}

			Rml::Element* element = document->GetElementById(segments[1]);

			if (element == nullptr)
			{
				return;
			}

			element->SetClass("hidden", false);
		}
	}

	void Event::OnAttach(Rml::Element* element)
	{
	}

	void Event::OnDetach(Rml::Element* element)
	{
		delete this;
	}

	Event::Event(const Rml::String& value) :
		value(value)
	{
	}
}
