#include "Gecko/UI/Widgets/Players.hpp"

#include "Gecko/Managers/PlayerManager.hpp"
#include "Gecko/Players/Player.hpp"

namespace Gecko
{
    void PlayersWidget::init_data_bindigs(Rml::Context* context)
    {
        if (Rml::DataModelConstructor constructor = context->CreateDataModel("players"))
        {
            if (Rml::StructHandle handle = constructor.RegisterStruct<Player>())
            {
                handle.RegisterMember("id", &Player::id);
                handle.RegisterMember("name", &Player::name);
                handle.RegisterMember("color", &Player::color);

                constructor.RegisterArray<Rml::Vector<Player>>();
                constructor.Bind("players", &m_players);

                m_model = constructor.GetModelHandle();
            }
        }
    }

    void PlayersWidget::update()
    {
        m_players.clear();

        for (const auto& [id, player] : PlayerManager::getSingleton())
        {
            m_players.push_back({ id.get(), player->get_name(), player->get_color()});
        }

        m_model.DirtyVariable("players");
    }
}
