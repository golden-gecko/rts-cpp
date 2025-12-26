#include "Gecko/UI/SelectionBox.hpp"

#include "Gecko/Games/Game.hpp"
#include "Gecko/Utils/Convert.hpp"

namespace Gecko
{
    SelectionBox::SelectionBox()
    {
        manual_object = Game::getSingleton().create_manual_object();
        manual_object->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
        manual_object->setRenderQueueGroup(Ogre::RenderQueueGroupID::RENDER_QUEUE_OVERLAY);
        manual_object->setUseIdentityProjection(true);
        manual_object->setUseIdentityView(true);

        scene_node = Game::getSingleton().create_scene_node();
        scene_node->attachObject(manual_object);

        set_visible(false);
    }

    SelectionBox::~SelectionBox()
    {
        Game::getSingleton().destroy_manual_object(manual_object);
        Game::getSingleton().destroy_scene_node(scene_node);
    }

    void SelectionBox::update()
    {
        auto left = start.x;
        auto top = start.y;
        auto right = end.x;
        auto bottom = end.y;

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

            manual_object->clear();
            manual_object->begin("selection_green", Ogre::RenderOperation::OperationType::OT_LINE_STRIP);

            manual_object->position(rect.left, rect.top, 0.0f);
            manual_object->normal(Ogre::Vector3::UNIT_Y);
            manual_object->textureCoord(0.0f, 0.0f);

            manual_object->position(rect.right, rect.top, 0.0f);
            manual_object->normal(Ogre::Vector3::UNIT_Y);
            manual_object->textureCoord(1.0f, 0.0f);

            manual_object->position(rect.right, rect.bottom, 0.0f);
            manual_object->normal(Ogre::Vector3::UNIT_Y);
            manual_object->textureCoord(1.0f, 1.0f);

            manual_object->position(rect.left, rect.bottom, 0.0f);
            manual_object->normal(Ogre::Vector3::UNIT_Y);
            manual_object->textureCoord(0.0f, 1.0f);

            manual_object->position(rect.left, rect.top, 0.0f);
            manual_object->normal(Ogre::Vector3::UNIT_Y);
            manual_object->textureCoord(0.0f, 0.0f);

            manual_object->end();
            manual_object->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);
        }

        set_visible(valid);
    }
}
