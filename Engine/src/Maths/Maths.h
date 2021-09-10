#pragma once

#include "HTypes.h"
#include "glm/glm.hpp"

namespace HEngine {
namespace Math {
    bool DecomposeTransform(const Mat4& transform, Vec3& Translation, Vec3& Rotation, Vec3& Scale);

}
}
