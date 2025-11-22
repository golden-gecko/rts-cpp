        /*
        TODO: Fix.
        // Get previous position.
        auto previous_position = get_position();

        // Set position.
        base_type::set_position(position);

        // Get current position.
        auto current_position = get_position();

        // Update terrain layer.
        auto& terrain = Game::getSingleton().get_active_map()->get_terrain();

        auto previous_index = terrain.get_index(previous_position);
        auto current_index = terrain.get_index(current_position);

        if (previous_index != current_index)
        {
            terrain.get_layer(LayerType::Value::Traffic)->as<TerrainLayerTraffic>()->substract(previous_index, 1);
            terrain.get_layer(LayerType::Value::Traffic)->as<TerrainLayerTraffic>()->add(current_index, 1);
        }
        */


       
        if (0)
        {
            // Get distance to next point in path.
            Ogre::Vector3 position = get_position();
            position.y = 0.0f;
            Ogre::Vector3 distance = path.front() - position;

            // Set direction.
            Ogre::Vector3 direction = distance.normalisedCopy();
            // TODO: Count turns.
            set_direction(direction);

            // TODO: Get speed from Drive component.
            // Get distance that we can travel in this frame.
            Ogre::Vector3 nextDistance = direction * 20.0f * speed * time;

            // If travel distance is greater than distance to next point in path,
            // move to next point in path. Otherwise, move to maximal distance.
            if (distance.squaredLength() > nextDistance.squaredLength())
            {
                Ogre::Vector3 position = get_position();
                position.y = 0.0f;
                Ogre::Vector3 nextPosition = position + nextDistance;

                // TODO: Add function or data type to simplify conversion between float and int coordinates.
                // TODO: Fix.
                if (false) // terrain.get_layer(LayerType::Value::Type)->as<TerrainLayerType>()->get_data(nextPosition.x / terrain_scale.x, nextPosition.z / terrain_scale.z) == TerrainLayerTypeType::Road)
                {
                    set_position(get_position() + nextDistance);
                    distance += nextDistance.length();

                    /*
                    // Produce pollution.
                    float pollution = airPollutionProduction * time;

                    // TODO: Refactor.
                    ObjectManager::getSingleton().get<Terrain>()[0]->dataLayers[TerrainLayer::Type::AirPollution]->add(getPosition(), pollution);

                    // TODO: Now, send only when vehicle passed through node center.
                    // Send traffic data to layer.
                    // TODO: Refactor.
                    // map->get<Terrain>()[0]->dataLayers[TerrainLayer::Type::Traffic]->add(
                    //     path.front(),
                    //     1.0f,
                    //     map->getClock().get_current()
                    // );
                    */
                }
                else
                {
                    // Computed path is no longer available.
                    path.clear();
                }
            }
            else
            {
                Ogre::Vector3 position = get_position();
                position.y = 0.0f;
                Ogre::Vector3 nextPosition = position + distance;

                // TODO: Fix.
                if (false) // terrain.get_layer(LayerType::Value::Type)->as<TerrainLayerType>()->get_data(nextPosition.x / terrain_scale.x, nextPosition.z / terrain_scale.z) == TerrainLayerTypeType::Road)
                {
                    Ogre::Vector3 position = get_position();
                    position.y = 0.0f;
                    set_position(position + distance);
                    distance += distance.length();

                    /*
                    // Produce pollution.
                    float pollution = airPollutionProduction * time;

                    // TODO: Refactor.
                    ObjectManager::getSingleton().get<Terrain>()[0]->dataLayers[TerrainLayer::Type::AirPollution]->add(getPosition(), pollution);

                    // Send traffic data to layer.
                    // TODO: Refactor.
                    ObjectManager::getSingleton().get<Terrain>()[0]->dataLayers[TerrainLayer::Type::Traffic]->add(
                        path.front(),
                        1.0f,
                        map->getClock().get_current()
                    );
                    */

                    path.pop_front();
                }
                else
                {
                    // Computed path is no longer available.
                    path.clear();
                }
            }
        }


    // TODO: Now, if vehicle can move further than current point in path, it will move only to current that point, but such movement is not smooth.
    // TODO: Vehicle should move as far as it can go in current frame. We have to compute next position based on next points in path.
    // TODO: Vehicles should not pass through each other. Faster vehicles should slow down (no overtaking will be allowed - maybe later).
    // TODO: Vehicle in traffic jam should wait a little after vehicle in front of them moved before moving.
    // TODO: In other words, distance at which vehicles will stop have to be shorter than distance at which vehicles will start to move again.
    void Vehicle::processMove(float time)
    {
        /*
        TODO: Fix.
        auto& terrain = Game::getSingleton().get_active_map()->get_terrain();
        auto terrain_scale = terrain.get_scale();
        */

        /*
        // Get path to target.
        if (path.size() == 0)
        {
            path = map->getPath(position, target);

            if (path.size() > 0)
            {
                setPosition(path.front());

                // TODO: Is this good fix?
                // TODO: It is needed, because last point in path is not equal to target, but it is shifted (car moves on the right side of the road).
                target = path.back();

                // Send data to data layer.
                // TODO: Fix.
                // TODO: Should we count also starting and ending point? Each start/end point very soon goes red.
                map->get<Terrain>()[0]->dataLayers[TerrainLayer::Type::Traffic]->add(
                    path.front(),
                    1.0f,
                    map->getClock().get_current()
                );
            }
        }
        */
    }


        /*
        if (0 && get_initialized() && is_enabled())
        {
            if (get_orders()->size() <= 0)
            {
                return;
            }

            auto order = OrderManager::getSingleton().get(get_orders()->front());

            switch (order->get_type())
            {
            case order_type::Value::Move:
            {
                auto order_move = dynamic_cast<OrderMove*>(order);

                if (get_state() != State::Value::Moving)
                {
                    auto position = get_position();
                    auto destination = order_move->get_target_position();

                    // TODO: Fix.
                    // path = Game::getSingleton().get_active_map()->get_terrain().get_path(position, destination);

                    if (path.size() <= 0)
                    {
                        // TODO: Here we should go into "Waiting" state, log information about no route and wait for route or order change.
                        throw Exception("No path to target.");

                        get_orders()->remove_current_order();
                    }
                    else
                    {
                        set_position(path.front());
                        setState(State::Value::Moving);
                    }
                }

                break;
            }
            }

            switch (get_state())
            {
            case State::Value::Moving:
                if (path.size() <= 0)
                {
                    get_orders()->remove_current_order();

                    setState(State::Value::Idle);
                }
                else
                {
                    processMove(time);
                }
                break;

                // TODO: Fix.
                Ogre::Vector3 position = getPosition();
                Ogre::Vector3 distance = order->getDestinationPosition() - position;

                // Target reached.
                if (distance.squaredLength() <= 0.1f)
                {
                    // TODO: Temporary fix, processMove method should remove last point and change state into idle.
                    path.clear();
                    setState(State::Idle);
                    break;
                }

                case State::Returning:
                    break;

                case State::MovingToFactory:
                    // Target reached.
                    if (distance.squaredLength() <= 0.1f)
                    {
                        // TODO: Temporary fix, processMove method should remove last point and change state into idle.
                        path.clear();

                        // Loads goods if we have more goods than truck capacity, so trunk will not work without full load.
                        // Loads goods if we have more goods than truck capacity, so trunk will not work without full load.
                        float goodsToLoad = maxGoods - goods;

                        auto factory = dynamic_cast<CityBuilding*>(targetObject);

                        if (factory->getGoods() < goodsToLoad)
                        {
                            setState(State::Idle);
                            break;
                        }

                        auto building = map->getBuildingWithoutGoods();

                        if (building == nullptr)
                        {
                            setState(State::Idle);
                            break;
                        }

                        auto entrance = building->get_entrance();

                        if (entrance)
                        {
                            factory->setGoods(factory->getGoods() - goodsToLoad);

                            setGooods(getGoods() + goodsToLoad);
                            setTarget(building);
                            setTarget(entrance.get().position);
                            setState(State::Transporting);
                        }
                        else
                        {
                            // TODO: Add log class.
                            std::cout << "One or more of target buildings have no entrance." << std::endl;
                        }
                        break;
                    }

                    processMove(time);
                    break;

                case State::Transporting:
                    // Target reached.
                    if (distance.squaredLength() <= 0.1f)
                    {
                        // TODO: Temporary fix, processMove method should remove last point and change state into idle.
                        // TODO: Also, this will not count end point into traffic density.
                        path.clear();

                        // Unload goods.
                        Building* building = dynamic_cast<CityBuilding*>(targetObject);

                        if (building != nullptr)
                        {
                            float goodsNeeded = building->getMaxGoods() - building->getGoods();

                            if (goods > goodsNeeded)
                            {
                                building->setGoods(building->getGoods() + goodsNeeded);
                                goodsDelivered += goodsNeeded;
                                goods -= goodsNeeded;
                            }
                            else
                            {
                                building->setGoods(building->getGoods() + goods);
                                goodsDelivered += goods;
                                goods -= goods;
                            }
                        }

                        setState(State::Idle);

                        // TODO: Unused code. Will be moved to orders (when implemented).
                        // auto buildingEntrance = building->get_entrance();
                        // auto factoryEntrance = factory->get_entrance();
                        //
                        // if (buildingEntrance && factoryEntrance)
                        // {
                        //     setPosition(buildingEntrance.get().position);
                        //     setDirection(buildingEntrance.get().direction.crossProduct(Ogre::Vector3::NEGATIVE_UNIT_Y));
                        //     setTarget(factory);
                        //     setTarget(factoryEntrance.get().position);
                        //     setState(State::Returning);
                        // }
                        // else
                        // {
                        //     // TODO: Add log class.
                        //     std::cout << "One or more of target buildings have no entrance." << std::endl;
                        // }
                        break;
                    }

                    processMove(time);
                    break;
            }

            // TODO: Set position to right side of the road.
            // TODO: Maybe we should set those positions after path is generated.
            // TODO: Before move, check if road still exists (or move off road, but very slowly).
            // TODO: Moving off road could generate mud roads (little faster over time).
            // TODO: Vehicle should not jump, to first position from path. It should drive there.
            // TODO: If vehicle is not on road, it should drive to nearest road.
        }
        */
