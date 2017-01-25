#pragma once

#include "Config.hpp"
#include "Simd.hpp"
#include "Floats.hpp"
#include <iosfwd>

namespace Fancy::Math
{
    template<typename T>
    class Vector4;

    template<>
    class Vector4<float>
    {
    public:
        Simd::Floats floats_;
        static const Vector4 Zero;
        static const Vector4 UnitX;
        static const Vector4 UnitY;
        static const Vector4 UnitZ;
        static const Vector4 UnitW;
        static const Vector4 One;
        static const Vector4 Infinity;
        static const Vector4 qNaN;
        static const Vector4 Bias4;

        //constructors
        Vector4() noexcept = default;
        Vector4(Simd::Floats floats) noexcept;
        explicit Vector4(const AlignedFloat4& xyzw) /*noexcept*/;
        explicit Vector4(const Float4& xyzw) noexcept;
        explicit Vector4(float w) noexcept;
        Vector4(float x, float y, float z, float w) noexcept;
        Vector4(const Vector4&) noexcept = default;
        Vector4& operator=(const Vector4&) noexcept = default;

        operator Simd::Floats() const noexcept;
        
        void WriteTo(AlignedFloat4& xyzw) const /*noexcept*/;
        void WriteTo(Float4& xyzw) const noexcept;
        void WriteTo(AlignedFloat3& xyz) const /*noexcept*/;
        void WriteTo(Float3& xyz) const noexcept;

        DefGetter(GetFloats, Simd::Floats, floats_)
        DefSetter(SetFloats, Simd::Floats, floats_)

        float X() const noexcept;
        float Y() const noexcept;
        float Z() const noexcept;
        float W() const noexcept;

        void SetX(float x) noexcept;
        void SetY(float y) noexcept;
        void SetZ(float z) noexcept;
        void SetW(float w) noexcept;
    };

    Vector4<float> MakePoint3D(Float3 point) noexcept;
    Vector4<float> MakeVector3D(Float3 vector) noexcept;

    Vector4<float> __vectorcall Max(Vector4<float> lhs, Vector4<float> rhs) noexcept;

    Vector4<float> __vectorcall operator- (Vector4<float> lhs) noexcept;

    Vector4<float> __vectorcall operator+ (Vector4<float> lhs, Vector4<float> rhs) noexcept;
    Vector4<float> __vectorcall operator- (Vector4<float> lhs, Vector4<float> rhs) noexcept;
    Vector4<float> __vectorcall operator* (Vector4<float> lhs, Vector4<float> rhs) noexcept;
    Vector4<float> __vectorcall operator* (Vector4<float> lhs, float x) noexcept;
    Vector4<float> __vectorcall operator/ (Vector4<float> lhs, Vector4<float> rhs) noexcept;
    Vector4<float> __vectorcall operator/ (Vector4<float> lhs, float y) noexcept;
    Vector4<float> __vectorcall operator& (Vector4<float> lhs, Vector4<float> rhs) noexcept;

    bool __vectorcall Equal4(Vector4<float> lhs, Vector4<float> rhs) noexcept;
    bool __vectorcall NearEqual4(Vector4<float> lhs, Vector4<float> rhs) noexcept;
    bool __vectorcall Greater4(Vector4<float> lhs, Vector4<float> rhs) noexcept;
    bool __vectorcall Less4(Vector4<float> lhs, Vector4<float> rhs) noexcept;

    bool __vectorcall Equal3(Vector4<float> lhs, Vector4<float> rhs) noexcept;
    bool __vectorcall NearEqual3(Vector4<float> lhs, Vector4<float> rhs) noexcept;
    bool __vectorcall Greater3(Vector4<float> lhs, Vector4<float> rhs) noexcept;
    bool __vectorcall Less3(Vector4<float> lhs, Vector4<float> rhs) noexcept;

    float __vectorcall Length3(Vector4<float> vector) noexcept;
    Vector4<float> __vectorcall Normalize3(Vector4<float> vector) noexcept;
    float __vectorcall Dot3(Vector4<float> lhs, Vector4<float> rhs) noexcept;
    Vector4<float> __vectorcall Cross3(Vector4<float> lhs, Vector4<float> rhs) noexcept;

    std::ostream& operator<< (std::ostream& os, const Vector4<float>& vec);
}
