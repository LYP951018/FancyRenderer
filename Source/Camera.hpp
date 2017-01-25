#pragma once

#include "Floats.hpp"
#include "Def.hpp"

namespace Fancy::Graphics
{
    class Camera
    {
    public:
        Camera(float nearZ, float farZ, float fov, float aspectRatio,
            Math::Float3 eye, Math::Float3 focus, Math::Float3 up) noexcept
            : NearZ{nearZ}, FarZ{farZ}, Fov{fov}, AspectRatio{aspectRatio},
            Eye{eye}, Focus{focus}, Up{up}
        {
        }

        float NearZ, FarZ;
        float Fov;
        float AspectRatio;
        Math::Float3 Eye, Focus, Up;

    private:
        Math::Float4x3 perspective_;
        Math::Float4x3 uvn_;

    public:
        DefGetter(Perspective, Math::Float4x3, perspective_)
        DefGetter(Uvn, Math::Float4x3, uvn_)
    };
}