#include "Gecko/UI/Widgets/SelectionBox.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Utils/Convert.hpp"

namespace Gecko
{
    void SelectionBoxWidget::ProcessEvent(Rml::Event& event)
    {
    }

    SelectionBoxWidget::SelectionBoxWidget()
    {
        m_manual_object = Game::getSingleton().create_manual_object();
        m_manual_object->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
        m_manual_object->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_OVERLAY);
        m_manual_object->setUseIdentityProjection(true);
        m_manual_object->setUseIdentityView(true);

        m_scene_node = Game::getSingleton().create_scene_node();
        m_scene_node->attachObject(m_manual_object);

        set_visible(false);
    }

    SelectionBoxWidget::~SelectionBoxWidget()
    {
        Game::getSingleton().destroy_manual_object(m_manual_object);
        Game::getSingleton().destroy_scene_node(m_scene_node);
    }

    void SelectionBoxWidget::update()
    {
        auto left = m_start.x;
        auto top = m_start.y;
        auto right = m_end.x;
        auto bottom = m_end.y;

        if (left > right)
        {
            std::swap(left, right);
        }

        if (top > bottom)
        {
            std::swap(top, bottom);
        }

        auto valid = is_valid();

        if (valid)
        {
            auto rect = Utils::Convert::to_rectangle_coordinates(Ogre::RealRect(left, top, right, bottom));

            m_manual_object->clear();
            m_manual_object->begin("selection_green", Ogre::RenderOperation::OperationType::OT_LINE_STRIP);

            m_manual_object->position(rect.left, rect.top, 0.0f);
            m_manual_object->normal(Ogre::Vector3::UNIT_Y);
            m_manual_object->textureCoord(0.0f, 0.0f);

            m_manual_object->position(rect.right, rect.top, 0.0f);
            m_manual_object->normal(Ogre::Vector3::UNIT_Y);
            m_manual_object->textureCoord(1.0f, 0.0f);

            m_manual_object->position(rect.right, rect.bottom, 0.0f);
            m_manual_object->normal(Ogre::Vector3::UNIT_Y);
            m_manual_object->textureCoord(1.0f, 1.0f);

            m_manual_object->position(rect.left, rect.bottom, 0.0f);
            m_manual_object->normal(Ogre::Vector3::UNIT_Y);
            m_manual_object->textureCoord(0.0f, 1.0f);

            m_manual_object->position(rect.left, rect.top, 0.0f);
            m_manual_object->normal(Ogre::Vector3::UNIT_Y);
            m_manual_object->textureCoord(0.0f, 0.0f);

            m_manual_object->end();
            m_manual_object->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
        }

        set_visible(valid);
    }
}
