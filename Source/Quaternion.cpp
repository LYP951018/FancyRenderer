#include "Quaternion.hpp"

namespace Fancy::Math
{
    Quaternion __vectorcall operator*(Quaternion lhs, Quaternion rhs) noexcept
    {
        // [ (Q2.w * Q1.x) + (Q2.x * Q1.w) + (Q2.y * Q1.z) - (Q2.z * Q1.y),
        //   (Q2.w * Q1.y) - (Q2.x * Q1.z) + (Q2.y * Q1.w) + (Q2.z * Q1.x),
        //   (Q2.w * Q1.z) + (Q2.x * Q1.y) - (Q2.y * Q1.x) + (Q2.z * Q1.w),
        //   (Q2.w * Q1.w) - (Q2.x * Q1.x) - (Q2.y * Q1.y) - (Q2.z * Q1.z) ]
        const auto sign1 = Vector4<float>{ 1.f, -1.f, 1.f, 1.f };
        const auto sign2 = Vector4<float>{ 1.f, 1.f, -1.f, -1.f };
        const auto sign3 = Vector4<float>{ -1.f, 1.f, 1.f, -1.f };
        const auto x = Vector4<float>{ Simd::DupX(rhs) };
        const auto y = Vector4<float>{ Simd::DupY(rhs) };
        const auto z = Vector4<float>{ Simd::DupZ(rhs) };
        const auto w = Vector4<float>{ Simd::DupW(rhs) };
        auto first = w * lhs.GetFloats();
        Simd::Floats temp = Simd::PermuteFloats<3, 2, 1, 0>(lhs.GetFloats());
        auto second = sign1 * x * temp;
        temp = Simd::PermuteFloats<2, 3, 0, 1>(lhs.GetFloats());
        auto third = sign2 * y * temp;
        auto fourth = sign3 * z * Simd::PermuteFloats<1, 0, 3, 2>(lhs.GetFloats());
        return Quaternion{ first + second + third + fourth };
    }

    Vector4<float> Rotate(Vector4<float> vector, Quaternion quaternion)
    {
        return Vector4<float>::Zero;
    }

    Quaternion::Quaternion(const Float4& xyzscalar) noexcept
        : floats_{ Simd::FloatsFromMemory(reinterpret_cast<const float*>(&xyzscalar)) }
    {}
}

