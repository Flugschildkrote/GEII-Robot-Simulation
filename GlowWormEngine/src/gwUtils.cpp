#include "gwUtils.h"

using namespace gwe::utils;

std::string gwe::utils::glmToStr(const glm::vec3 &vec)
{
    std::stringstream ss;
    ss << "[" << vec.x << " ; " << vec.y << " ; " << vec.z << "]";
    return ss.str();
}


std::string gwe::utils::glmToStr(const glm::vec4 &vec)
{
    std::stringstream ss;
    ss << "[" << vec.x << " ; " << vec.y << " ; " << vec.z << " ; " << vec.w << "]";
    return ss.str();
}
