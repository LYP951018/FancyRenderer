#pragma once

#include "Floats.hpp"
#include "Matrix.hpp"
#include "Def.hpp"

namespace Fancy::Math
{
    class Quaternion
    {
    public:
        static const Quaternion Identity;

        Simd::Floats floats_;

        explicit Quaternion(const Float4& xyzscalar) noexcept;

        explicit Quaternion(Simd::Floats floats) noexcept
            : floats_{floats}
        {}

        DefGetter(GetFloats, Simd::Floats, floats_)

        void Conjugate() noexcept;
        void Inverse() noexcept;

        operator Simd::Floats() const noexcept
        {
            return floats_;
        }

        static Quaternion FromAxisAngle(Float3 axis, float angle) noexcept;
        static Quaternion FromRotationMatrix(Matrix rotation) noexcept;
    };

    Quaternion __vectorcall operator* (Quaternion lhs, Quaternion rhs) noexcept;

    /*Quaternion Conjugate(Quaternion quaternion) noexcept;
    Quaternion Inverse(Quaternion quaternion) noexcept;
    Matrix GetRotationMatrix(Quaternion quaternion) noexcept;*/
    Vector4<float> Rotate(Vector4<float> vector, Quaternion quaternion);
}