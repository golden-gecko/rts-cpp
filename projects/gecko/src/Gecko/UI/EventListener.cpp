#include "Gecko/UI/EventListener.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Log.hpp"
#include "Gecko/UI/UI.hpp"

namespace Gecko
{
	void EventListener::ProcessEvent(Rml::Event& event)
	{
		L_TRACE << "EventListener::ProcessEvent()";

		std::vector<std::string> segments;

        boost::algorithm::split(segments, value, boost::is_any_of(":"), boost::token_compress_on);

		if (segments.size() != 2)
		{
			return;
		}

		const std::string function = segments[0];
		const std::string argument = segments[1];

		if (function == "call")
		{
			if (argument == "quit-to-menu")
			{
				Game::getSingleton().load_map("menu");
			}
			else if (argument == "quit-to-desktop")
			{
				Game::getSingleton().quit();
			}
		}
		else if (function == "open")
		{
			Rml::ElementDocument* document = UI::getSingleton().get_document();

			if (document == nullptr)
			{
				return;
			}

			Rml::Element* element = document->GetElementById(argument);

			if (element == nullptr)
			{
				return;
			}

			element->SetClass("hidden", false);
		}
	}

	void EventListener::OnAttach(Rml::Element* element)
	{
	}

	void EventListener::OnDetach(Rml::Element* element)
	{
		delete this;
	}

	EventListener::EventListener(const Rml::String& value) :
		value(value)
	{
	}
}
