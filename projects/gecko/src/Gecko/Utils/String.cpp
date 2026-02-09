#include "Gecko/Utils/String.hpp"

namespace Gecko::Utils::String
{
    std::string to_title(const std::string& value)
    {
        std::string result = value;
        
        boost::algorithm::replace_all(result, "_", " ");
        boost::algorithm::to_upper(result);

        return result;
    }
}
