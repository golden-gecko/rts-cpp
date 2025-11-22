        /*
        TODO: Refactor.
        set_selected(false);
        set_visible(false);

        // TODO: Move to configuration->
        if (type != Type::Value::Missile)
        {
            // map->get_terrain().remove_unit_from_position(get_id(), get_position());

            auto player = PlayerManager::getSingleton().get(get_player_id());

            if (player)
            {
                player->remove_unit_from_position(get_id(), get_position(), get_visibility_range());
            }
        }
        */

               /*
        map->get_terrain().update_unit_position(get_id(), get_position(), p);

        auto player = PlayerManager::getSingleton().get(get_player_id());

        if (player)
        {
        player->update_unit_position(get_id(), get_position(), p, get_visibility_range());
        }
        }
        */
       
            // Debug render.
            Ogre::Quaternion q = scene_node->_getDerivedOrientation();
            Ogre::Vector3 current_direction = q * Ogre::Vector3(0, 0, -1);

            // m->clear();
            m->begin("red", Ogre::RenderOperation::OT_LINE_LIST);

            m->position(new_position);
            m->normal(Ogre::Vector3::UNIT_Y);

            m->position(new_position + new_normal * 10.0f);
            m->normal(Ogre::Vector3::UNIT_Y);

            m->position(new_position);
            m->normal(Ogre::Vector3::UNIT_Y);

            m->position(new_position + current_direction * 10.0f);
            m->normal(Ogre::Vector3::UNIT_Y);

            m->end();

            if (m->isAttached() == false)
            {
                s->attachObject(m);
            }
			
			        static auto m = Game::getSingleton().create_manual_object();
        static auto s = Game::getSingleton().create_scene_node();
