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

		if (value == "call:quit-to-menu")
		{
			on_quit_to_menu();
		}
		else if (value == "call:quit-to-desktop")
		{
			on_quit_to_desktop();
		}
		else if (function == "close")
		{
			on_close(argument);
		}
		else if (function == "open")
		{
			on_open(argument);
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

	Rml::Element* EventListener::get_element(const std::string& element) const
	{
		Rml::ElementDocument* document = UI::getSingleton().get_document();

		if (document == nullptr)
		{
			return nullptr;
		}

		return document->GetElementById(element);
	}

	void EventListener::on_close(const std::string& value) const
	{
		Rml::Element* element = get_element(value);

		if (element == nullptr)
		{
			return;
		}

		element->SetClass("hidden", true);
	}

	void EventListener::on_open(const std::string& value) const
	{
		Rml::Element* element = get_element(value);

		if (element == nullptr)
		{
			return;
		}

		element->SetClass("hidden", false);
	}

	void EventListener::on_quit_to_menu() const
	{
		Game::getSingleton().load_map("menu");
	}

	void EventListener::on_quit_to_desktop() const
	{
		Game::getSingleton().quit();
	}
}
