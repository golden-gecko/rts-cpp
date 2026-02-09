#pragma once

namespace Gecko::Key
{
    std::string to_string(OIS::KeyCode value);

    OIS::KeyCode from_string(const std::string& value);
}
