#pragma once

#include "glm/glm.hpp"
#include "HTypes.h"

namespace HEngine
{
    namespace Math
    {
        bool DecomposeTransform(const Mat4& transform, Vec3& Translation, Vec3& Rotation, Vec3& Scale);



    }
}