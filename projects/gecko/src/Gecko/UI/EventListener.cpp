#include "Gecko/UI/EventListener.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/UI/UI.hpp"
#include "Gecko/UI/Widgets/Configurations.hpp"
#include "Gecko/UI/Widgets/Orders.hpp"
#include "Gecko/UI/Widgets/Skills.hpp"

namespace Gecko
{
	void EventListener::ProcessEvent(Rml::Event& event)
	{
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

	void EventListener::on_menu(const std::string& value) const
	{
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

	void EventListener::on_quit_to_menu() const
	{
		Game::getSingleton().load_map("menu");
	}

	void EventListener::on_quit_to_desktop() const
	{
		Game::getSingleton().quit();
	}
}
