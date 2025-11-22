if (time_scale > 0)
{
    // Faster.
    for (auto i = 0; i < time_scale; i++)
    {
        MapManager::getSingleton().update(time);
        ObjectManager::getSingleton().update(time);
        OrderManager::getSingleton().update(time);
        PlayerManager::getSingleton().update(time);
    }
}
else if (time_scale != -2)
{
    // Slower.
    auto scaled_time = time * get_time_scale();

    MapManager::getSingleton().update(scaled_time);
    ObjectManager::getSingleton().update(scaled_time);
    OrderManager::getSingleton().update(scaled_time);
    PlayerManager::getSingleton().update(scaled_time);
}

void Game::toggle_pause()
{
    if (time_scale == -2)
    {
        time_scale = 1;
    }
    else
    {
        time_scale = -2;
    }
}

void faster()
{
    if (time_scale < 2)
    {
        ++time_scale;
    }
}

void slower()
{
    if (time_scale > -2)
    {
        --time_scale;
    }
}

auto get_time_scale() const
{
    if (time_scale == -2)
    {
        return 0.0f;
    }

    if (time_scale == -1)
    {
        return 0.25f;
    }

    if (time_scale == 0)
    {
        return 0.5f;
    }

    return static_cast<float>(time_scale);
}

int time_scale = 1;
