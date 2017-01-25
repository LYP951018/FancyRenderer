#include "Vectors.hpp"
#include "Math.hpp"
#include <ostream>
#include <gsl/gsl_assert>

namespace Fancy::Math
{
    const Vector4<float> Vector4<float>::Zero{ Simd::ZeroFloats() };
    const Vector4<float> Vector4<float>::UnitX = { 1.0f, 0.f, 0.f, 0.f };
    const Vector4<float> Vector4<float>::UnitY = { 0.f, 1.f, 0.f, 0.f };
    const Vector4<float> Vector4<float>::UnitZ = { 0.f, 0.f, 1.f, 0.f };
    const Vector4<float> Vector4<float>::UnitW = { 0.f, 0.f, 0.f, 1.f };
    const Vector4<float> Vector4<float>::One = { 1.f, 1.f, 1.f, 1.f };
    const Vector4<float> Vector4<float>::Infinity = Simd::AsFloats(Simd::MakeInts(0x7F800000));
    const Vector4<float> Vector4<float>::qNaN = Simd::AsFloats(Simd::MakeInts(0x7FC00000));
    const Vector4<float> Vector4<float>::Bias4 = Vector4<float>{ 1e-6f };

    const Vector4<float> IgnoreWMask = Simd::AsFloats(Simd::MakeInts(0xFFFFFFFF));

    Vector4<float>::Vector4(Simd::Floats floats) noexcept
    {
        floats_ = floats;
    }

    Vector4<float>::Vector4(const AlignedFloat4& xyzw) /*noexcept*/
        : Vector4{Simd::FloatsFromAlignedMemory(reinterpret_cast<const float*>(&xyzw))}
    {}

    Vector4<float>::Vector4(const Float4& xyzw) noexcept
        : Vector4{Simd::FloatsFromMemory(reinterpret_cast<const float*>(&xyzw))}
    {}

    Vector4<float>::Vector4(float x) noexcept
        : Vector4{ Simd::MakeFloats(x)}
    {}

    Vector4<float>::Vector4(float x, float y, float z, float w) noexcept
        : Vector4{ Simd::MakeFloats(x, y, z, w) }
    {}

    Vector4<float>::operator Simd::Floats() const noexcept
    {
        return GetFloats();
    }

    void Vector4<float>::WriteTo(AlignedFloat4 & xyzw) const /*noexcept*/
    {
        Simd::StoreAligned(GetFloats(), reinterpret_cast<float*>(&xyzw));
    }

    void Vector4<float>::WriteTo(Float4 & xyzw) const noexcept
    {
        Simd::Store(GetFloats(), reinterpret_cast<float*>(&xyzw));
    }

    float Vector4<float>::X() const noexcept
    {
        return Simd::First(GetFloats());
    }

    float Vector4<float>::Y() const noexcept
    {
        return Simd::Second(GetFloats());
    }

    float Vector4<float>::Z() const noexcept
    {
        return Simd::Third(GetFloats());
    }

    float Vector4<float>::W() const noexcept
    {
        return Simd::Fourth(GetFloats());
    }

    void Vector4<float>::SetX(float x) noexcept
    {
        SetFloats(Simd::SetFirst(GetFloats(), x));
    }

    void Vector4<float>::SetY(float y) noexcept
    {
        SetFloats(Simd::SetSecond(GetFloats(), y));
    }

    void Vector4<float>::SetZ(float z) noexcept
    {
        SetFloats(Simd::SetThird(GetFloats(), z));
    }

    void Vector4<float>::SetW(float w) noexcept
    {
        SetFloats(Simd::SetFourth(GetFloats(), w));
    }

    Vector4<float> MakePoint3D(Float3 point) noexcept
    {
        return { point.X, point.Y, point.Z, 1.f };
    }

    Vector4<float> MakeVector3D(Float3 vector) noexcept
    {
        return { vector.X, vector.Y, vector.Z, 0.f };
    }

    Vector4<float> __vectorcall Max(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return Simd::Max(lhs, rhs);
    }

    Vector4<float> __vectorcall operator-(Vector4<float> lhs) noexcept
    {
        return Simd::Neg(lhs);
    }

    Vector4<float> __vectorcall operator+ (Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return Simd::Add(lhs, rhs);
    }

    Vector4<float> __vectorcall operator-(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return Simd::Sub(lhs, rhs);
    }

    Vector4<float> __vectorcall operator*(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return Simd::Mul(lhs, rhs);
    }

    Vector4<float> __vectorcall operator*(Vector4<float> lhs, float x) noexcept
    {
        return Simd::Mul(lhs, Simd::MakeFloats(x));
    }

    Vector4<float> __vectorcall operator&(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return Simd::And(lhs, rhs);
    }

    bool __vectorcall Equal4(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return Simd::Msbs(Simd::Equal(lhs, rhs)) == 0xfu;
    }

    bool __vectorcall NearEqual4(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        const auto delta = lhs - rhs;
        return Less4(Max(delta, -delta), Simd::MakeFloats(1e-6f));
    }

    bool __vectorcall Greater4(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return Simd::Msbs(Simd::Greater(lhs, rhs)) == 0xfu;
    }

    bool __vectorcall Less4(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return Simd::Msbs(Simd::Less(lhs, rhs)) == 0xfu;
    }

    bool __vectorcall Equal3(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return (Simd::Msbs(Simd::Equal(lhs, rhs)) & 0x7u) == 0x7u;
    }

    bool __vectorcall NearEqual3(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        const auto delta = lhs - rhs;
        return Less3(Max(delta, -delta), Simd::MakeFloats(1e-6f));
    }

    bool __vectorcall Greater3(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return (Simd::Msbs(Simd::Greater(lhs, rhs)) & 0x7u) == 0x7u;
    }

    bool __vectorcall Less3(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return (Simd::Msbs(Simd::Less(lhs, rhs)) & 0x7u) == 0x7u;
    }

    float __vectorcall Length3(Vector4<float> vector) noexcept
    {
        return std::sqrt(Dot3(vector, vector));
    }

    Vector4<float> __vectorcall Normalize3(Vector4<float> vector) noexcept
    {
        auto lengthVec = Simd::Sqrt(Simd::Dot<true, true, true, false>(vector, vector));
        auto notZero = Simd::NotEqual(lengthVec, Vector4<float>::Zero);
        auto notInfinity = Simd::NotEqual(lengthVec, Vector4<float>::Infinity);
        vector = Simd::Div(vector, lengthVec);
        auto result = Simd::And(notZero, vector);
        auto temp1 = Simd::And(result, notInfinity);
        auto temp2 = Simd::NotAnd(notInfinity, Vector4<float>::qNaN);
        return Simd::Or(temp1, temp2);
    }

    float __vectorcall Dot3(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        return Simd::First(Simd::Dot<true, true, true, false>(lhs, rhs));
    }

    Vector4<float> __vectorcall Cross3(Vector4<float> lhs, Vector4<float> rhs) noexcept
    {
        auto v0 = Simd::PermuteFloats<1, 2, 0, 3>(lhs);
        auto v1 = Simd::PermuteFloats<2, 0, 1, 3>(rhs);
        //u2v3 u3v1 u1v2
        v0 = Simd::Mul(v0, v1);
        v1 = Simd::PermuteFloats<2, 0, 1, 3>(lhs);
        auto v2 = Simd::PermuteFloats<1, 2, 0, 3>(rhs);
        //u3v2 u1v3 u2v1
        v1 = Simd::Mul(v1, v2);
        v0 = Simd::Sub(v0, v1);
        return Simd::And(v0, IgnoreWMask);
    }

    std::ostream& operator<< (std::ostream& os, const Vector4<float>& vec)
    {
        return os << "<" << vec.X() << ", " << vec.Y() << ", " << vec.Z() << ", " << vec.W() << ">";
    }
}