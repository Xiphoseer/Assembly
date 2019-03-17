#include "types.hpp"

namespace assembly::common
{
    const vector3f vector3f::ZERO = {0,0,0};
    const quaternion quaternion::IDENTITY = {0,0,0,1};

    vector3f::vector3f(const float& x, const float& y, const float& z)
    : x(x), y(y), z(z) {}

    vector3f::vector3f(): x(0.0f), y(0.0f), z(0.0f) {}
}
