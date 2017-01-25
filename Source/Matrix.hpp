#pragma once

#include "Vectors.hpp"
#include <iosfwd>

namespace Fancy::Math
{
    struct alignas(16) Matrix
    {
        static const Matrix Identity;

        Vector4<float> Rows[4];

        Matrix() noexcept = default;

        Matrix(Vector4<float> r0, Vector4<float> r1, Vector4<float> r2, Vector4<float> r3) noexcept;

        Matrix(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) noexcept;

        Matrix(const Float4x3& floats) noexcept;
        Matrix(const AlignedFloat4x3& floats) noexcept;

        Matrix(const Matrix&) noexcept = default;
        Matrix& operator= (const Matrix&) noexcept = default;

        static Matrix FromTranslation(float x, float y, float z) noexcept;
        static Matrix __vectorcall FromTranslation(Vector4<float> offset) noexcept;
        static Matrix FromScale(float x, float y, float z) noexcept;
        static Matrix __vectorcall FromScale(Vector4<float> scale) noexcept;
        static Matrix FromRotationX(float angle) noexcept;
        static Matrix FromRotationY(float angle) noexcept;
        static Matrix FromRotationZ(float angle) noexcept;
        static Matrix __vectorcall FromLookAt(Vector4<float> eye, Vector4<float> target, Vector4<float> up) noexcept;
        static Matrix PerpspectiveFromFov(float fov, float aspectRatio, float nearZ, float farZ) noexcept;

        //void Decompose(Vector4& scale, Vector4& rotQuat, Vector4& trans) const noexcept;
        //bool IsIdentity() const noexcept;
    };

    Matrix __vectorcall Compose(Matrix lhs, const Matrix& rhs) noexcept;
    Matrix __vectorcall operator* (Matrix matrix, float n) noexcept;
    Matrix __vectorcall operator+ (Matrix lhs, const Matrix& rhs) noexcept;
    Matrix __vectorcall operator- (Matrix lhs, const Matrix& rhs) noexcept;

    Matrix Transpose(Matrix) noexcept;
    //float Determinant(Matrix m) noexcept;
    //Matrix Inverse(Matrix m) noexcept;

    bool __vectorcall operator== (Matrix lhs, const Matrix& rhs) noexcept;
    bool __vectorcall operator!= (Matrix lhs, const Matrix& rhs) noexcept;

    std::ostream& operator<< (std::ostream& os, Matrix matrix);

    Vector4<float> __vectorcall Transform4(Vector4<float> vector, Matrix matrix) noexcept;
}
