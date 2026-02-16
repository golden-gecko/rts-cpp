#include "Gecko/UI/Widgets/SelectionBox.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Players/Player.hpp"
#include "Gecko/Scenes/Scene.hpp"
#include "Gecko/Settings.hpp"
#include "Gecko/Utils/Convert.hpp"

namespace Gecko
{
    void SelectionBoxWidget::ProcessEvent(Rml::Event& event)
    {
    }

    SelectionBoxWidget::SelectionBoxWidget(const ScenePtr& scene) :
        m_scene(scene)
    {
        std::string color;

        if (PlayerPtr player = Game::getSingleton().get_active_player())
        {
            color = player->get_color();
        }
        else
        {
            color = Settings::Material::Default;
        }

        m_manual_object = m_scene->create_manual_object();
        m_manual_object->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
        m_manual_object->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_OVERLAY);
        m_manual_object->setUseIdentityProjection(true);
        m_manual_object->setUseIdentityView(true);

        m_manual_object->begin("selection_" + color, Ogre::RenderOperation::OperationType::OT_LINE_STRIP);
        m_manual_object->position(0.0f, 0.0f, 0.0f);
        m_manual_object->textureCoord(0.0f, 0.0f);
        m_manual_object->position(1.0f, 0.0f, 0.0f);
        m_manual_object->textureCoord(1.0f, 0.0f);
        m_manual_object->position(1.0f, 1.0f, 0.0f);
        m_manual_object->textureCoord(1.0f, 1.0f);
        m_manual_object->position(0.0f, 1.0f, 0.0f);
        m_manual_object->textureCoord(0.0f, 1.0f);
        m_manual_object->position(0.0f, 0.0f, 0.0f);
        m_manual_object->textureCoord(0.0f, 0.0f);
        m_manual_object->end();

        m_scene_node = m_scene->create_scene_node();
        m_scene_node->attachObject(m_manual_object);

        set_visible(false);
    }

    SelectionBoxWidget::~SelectionBoxWidget()
    {
        m_scene->destroy_manual_object(m_manual_object);
        m_scene->destroy_scene_node(m_scene_node);
    }

    void SelectionBoxWidget::update()
    {
        bool valid = is_valid();

        if (valid)
        {
            Ogre::Real left   = m_start.x;
            Ogre::Real top    = m_start.y;
            Ogre::Real right  = m_end.x;
            Ogre::Real bottom = m_end.y;

            if (left > right)
            {
                std::swap(left, right);
            }

            if (top > bottom)
            {
                std::swap(top, bottom);
            }

            Ogre::RealRect rect = Utils::Convert::to_rectangle_coordinates(Ogre::RealRect(left, top, right, bottom));

            m_manual_object->beginUpdate(0);
            m_manual_object->position(rect.left, rect.top, 0.0f);
            m_manual_object->position(rect.right, rect.top, 0.0f);
            m_manual_object->position(rect.right, rect.bottom, 0.0f);
            m_manual_object->position(rect.left, rect.bottom, 0.0f);
            m_manual_object->position(rect.left, rect.top, 0.0f);
            m_manual_object->end();
        }

        set_visible(valid);
    }

    bool SelectionBoxWidget::is_valid() const
    {
        Ogre::Real left   = m_start.x;
        Ogre::Real top    = m_start.y;
        Ogre::Real right  = m_end.x;
        Ogre::Real bottom = m_end.y;

        if (left > right)
        {
            std::swap(left, right);
        }

        if (top > bottom)
        {
            std::swap(top, bottom);
        }

        return ((bottom - top) > Settings::UI::SelectionBoxMinSize) && ((right - left) > Settings::UI::SelectionBoxMinSize);
    }

    bool SelectionBoxWidget::is_visible() const
    {
        return m_manual_object->isVisible();
    }
}
