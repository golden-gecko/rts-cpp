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

        boost::algorithm::split(segments, m_value, boost::is_any_of(":"), boost::token_compress_on);

		if (segments.size() != 2)
		{
			return;
		}

		std::string function = segments[0];
		std::string argument = segments[1];

		if (m_value == "call:quit-to-menu")
		{
			on_quit_to_menu();
		}
		else if (m_value == "call:quit-to-desktop")
		{
			on_quit_to_desktop();
		}
		else if (function == "close")
		{
			on_close(argument);
		}
		else if (function == "configuration")
		{
			on_configuration(argument);
		}
		else if (function == "menu")
		{
			on_menu(argument);
		}
		else if (function == "open")
		{
			on_open(argument);
		}
		else if (function == "order")
		{
			on_order(argument);
		}
		else if (function == "skill")
		{
			on_skill(argument);
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
		m_value(value)
	{
		L_TRACE << "EventListener::EventListener(" << value << ")";
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

		// TODO: Fix. Mouse is still detecting hidden object.
		element->SetClass("hidden", true);
	}

	void EventListener::on_configuration(const std::string& value) const
	{
	}

	void EventListener::on_menu(const std::string& value) const
	{
		L_TRACE << "EventListener::on_menu(" << value << ")";

		get_element("load-menu")->SetClass("hidden", true);
		get_element("main-menu")->SetClass("hidden", true);
		get_element("map-menu")->SetClass("hidden", true);

		if (value == "load-menu")
		{
			get_element("load-menu")->SetClass("hidden", false);
		}
		else if (value == "main-menu")
		{
			get_element("main-menu")->SetClass("hidden", false);
		}
		else if (value == "map-menu")
		{
			get_element("map-menu")->SetClass("hidden", false);
		}
	}

	void EventListener::on_open(const std::string& value) const
	{
		Rml::Element* element = get_element(value);

		if (element == nullptr)
		{
			return;
		}

		// TODO: Fix. Mouse is still detecting hidden object.
		element->SetClass("hidden", false);
	}

	void EventListener::on_order(const std::string& value) const
	{
		UI::getSingleton().set_order_type(order_type::from_string(value));
	}

	void EventListener::on_quit_to_menu() const
	{
		Game::getSingleton().load_map("menu");
	}

	void EventListener::on_quit_to_desktop() const
	{
		Game::getSingleton().quit();
	}

	void EventListener::on_skill(const std::string& value) const
	{
	}
}
