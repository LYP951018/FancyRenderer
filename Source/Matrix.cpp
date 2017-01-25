#include "Matrix.hpp"
#include <cmath>
#include <iostream>

namespace Fancy::Math
{
    const Matrix Matrix::Identity = { Vector4<float>::UnitX, Vector4<float>::UnitY, Vector4<float>::UnitZ, Vector4<float>::UnitW };

    template<std::uint8_t N>
    __forceinline void CalcRow(Matrix& lhs, const Matrix& rhs) noexcept
    {
        auto w = lhs.Rows[N];
        auto x = Simd::DupX(w);
        auto y = Simd::DupY(w);
        auto z = Simd::DupZ(w);
        w = Simd::DupW(w);
        lhs.Rows[N] = x * rhs.Rows[0] + y * rhs.Rows[1] + z * rhs.Rows[2] + w * rhs.Rows[3];
    }

    Matrix __vectorcall Compose(Matrix lhs, const Matrix& rhs) noexcept
    {
        CalcRow<0>(lhs, rhs);
        CalcRow<1>(lhs, rhs);
        CalcRow<2>(lhs, rhs);
        CalcRow<3>(lhs, rhs);
        return lhs;
    }

    Matrix __vectorcall operator+ (Matrix lhs, const Matrix& rhs) noexcept
    {       
        return {
            lhs.Rows[0] + rhs.Rows[0],
            lhs.Rows[1] + rhs.Rows[1],
            lhs.Rows[2] + rhs.Rows[2],
            lhs.Rows[3] + rhs.Rows[3]
        };
    }

    Matrix __vectorcall operator- (Matrix lhs, const Matrix & rhs) noexcept
    {
        return {
            lhs.Rows[0] - rhs.Rows[0],
            lhs.Rows[1] - rhs.Rows[1],
            lhs.Rows[2] - rhs.Rows[2],
            lhs.Rows[3] - rhs.Rows[3]
        };
    }

    //Use NearEqual.
    bool __vectorcall operator==(Matrix lhs, const Matrix & rhs) noexcept
    {
        const auto unroll = [&](std::size_t i) noexcept {
            return NearEqual4(lhs.Rows[i], rhs.Rows[i]);
        };
        return unroll(0) && unroll(1) && unroll(2) && unroll(3);
    }

    bool __vectorcall operator!=(Matrix lhs, const Matrix & rhs) noexcept
    {
        return !(lhs == rhs);
    }

    std::ostream & operator<<(std::ostream & os, Matrix matrix)
    {
        for (const auto& row : matrix.Rows)
        {
            os << row << "\n";
        }
        return os;
    }

    Vector4<float> __vectorcall Transform4(Vector4<float> vector, Matrix matrix) noexcept
    {
        auto x = Simd::DupX(vector);
        auto y = Simd::DupY(vector);
        auto z = Simd::DupZ(vector);
        auto w = Simd::DupW(vector);
        return x * matrix.Rows[0] + y * matrix.Rows[1]
            + z * matrix.Rows[2] + w * matrix.Rows[3];
    }

    Matrix::Matrix(Vector4<float> r0, Vector4<float> r1, Vector4<float> r2, Vector4<float> r3) noexcept
        : Rows{r0, r1, r2, r3}
    {}

    Matrix::Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) noexcept
        : Rows{{m00, m01, m02, m03}, {m10, m11, m12, m13}, {m20, m21, m22, m23}, {m30, m31, m32, m33}}
    {}

    Matrix Matrix::FromTranslation(float x, float y, float z) noexcept
    {
        return Matrix{
            Vector4<float>::UnitX, Vector4<float>::UnitY, Vector4<float>::UnitZ, {x, y, z, 1.f}
        };
    }

    Matrix __vectorcall Matrix::FromTranslation(Vector4<float> offset) noexcept
    {
        return FromTranslation(offset.X(), offset.Y(), offset.Z());
    }

    Matrix Matrix::FromScale(float x, float y, float z) noexcept
    {
        Matrix m;
        m.Rows[0] = { 0.f, 0.f, 0.f, x };
        m.Rows[1] = { 0.f, 0.f, y, 0.f };
        m.Rows[2] = { 0.f, z, 0.f, 0.f };
        m.Rows[3] = Vector4<float>::One;
        return m;
     } 

    Matrix __vectorcall Matrix::FromScale(Vector4<float> scale) noexcept
    {
        Matrix m;
        m.Rows[0] = scale & Vector4<float>::UnitX;
        m.Rows[1] = scale & Vector4<float>::UnitY;
        m.Rows[2] = scale & Vector4<float>::UnitZ;
        m.Rows[3] = Vector4<float>::One;
        return m;
    }

    Matrix Matrix::FromRotationX(float angle) noexcept
    {        
        const auto angleSin = std::sin(angle);
        const auto angleCos = std::cos(angle);
        return {
            Vector4<float>::UnitX,
            { 0.f, angleCos, -angleCos, 0.f },
            { 0.f, angleSin, angleCos, 0.f },
            Vector4<float>::UnitW
        };
    }

    Matrix Matrix::FromRotationY(float angle) noexcept
    {
        const auto angleSin = std::sin(angle);
        const auto angleCos = std::cos(angle);
        return {
            {angleCos, 0.f, -angleSin, 0.f},
            Vector4<float>::UnitY,
            {angleSin, 0.f, angleCos, 0.f},
            Vector4<float>::UnitW
        };
    }

    Matrix Matrix::FromRotationZ(float angle) noexcept
    {
        const auto angleSin = std::sin(angle);
        const auto angleCos = std::cos(angle);
        return{
            { angleCos, angleSin, 0.f, 0.f },
            { -angleSin, angleCos, 0.f, 0.f },
            Vector4<float>::UnitZ,          
            Vector4<float>::UnitW
        };
    }

    Matrix __vectorcall Matrix::FromLookAt(Vector4<float> eye, Vector4<float> target, Vector4<float> up) noexcept
    {
        const auto uAxis = target - eye;
        const auto vAxis = Normalize3(Cross3(uAxis, up));
        const auto nAxis = Normalize3(Cross3(uAxis, vAxis));
        return{ uAxis, vAxis, nAxis, Vector4<float>::Zero };
    }

    //right hand.
    //http://ogldev.atspace.co.uk/www/tutorial12/tutorial12.html
    Matrix Matrix::PerpspectiveFromFov(float fov, float aspectRatio, float nearZ, float farZ) noexcept
    {
        fov /= 2.f;
        const auto s = std::sin(fov);
        const auto c = std::cos(fov);
        const auto t = c / s;
        const auto delta = farZ / (nearZ - farZ);
        return {
            {t / aspectRatio, 0.f, 0.f, 0.f},
            {0.f, t, 0.f, 0.f},
            {0.f, 0.f, delta, -1.f},
            {0.f, 0.f, nearZ * delta, 0.f}
        };
    }

    Matrix Transpose(Matrix m) noexcept
    {
        _MM_TRANSPOSE4_PS(m.Rows[0], m.Rows[1], m.Rows[2], m.Rows[3]);
        return m;
    }

    //float Matrix::Inverse(Matrix m) noexcept
    //{
    //    //cofactor matrix:
    //    /*"fkp + gln + hjo - hkn - gjp - flo", "ekp + glm + hio - hkm - gip - elo",
    //    "ejp + flm + hin - hjm - fip - eln", "ejo + fkm + gin - gjm - fio - ekn",
    //    "bkp + cln + djo - dkn - cjp - blo", "akp + clm + dio - dkm - cip - alo",
    //    "ajp + blm + din - djm - bip - aln", "ajo + bkm + cin - cjm - bio - akn",
    //    "bgp + chn + dfo - dgn - cfp - bho", "agp + chm + deo - dgm - cep - aho",
    //    "afp + bhm + den - dfm - bep - ahn", "afo + bgm + cen - cfm - beo - agn",
    //    "bgl + chj + dfk - dgj - cfl - bhk", "agl + chi + dek - dgi - cel - ahk",
    //    "afl + bhi + dej - dfi - bel - ahj", "afk + bgi + cej - cfi - bek - agj"  */

    //    const auto abcd = Rows[0];

    //    const auto sign1 = Vector4{ 1.f, -1.f, 1.f, -1.f };
    //    const auto sign2 = Vector4{ -1.f, 1.f, -1.f, 1.f };

    //    const Vector4 temp[] = {
    //        //0 f e e e
    //        PermuteElements<1, 0, 0, 0>(Rows[1]),
    //        //1 k k j j
    //        PermuteElements<2, 2, 1, 1>(Rows[2]),
    //        //2 p p p o
    //        PermuteElements<3, 3, 3, 2>(Rows[3]),
    //        //3 g g f f
    //        PermuteElements<2, 2, 1, 1>(Rows[1]),
    //        //4 l l l k
    //        PermuteElements<3, 3, 3, 2>(Rows[2]),
    //        //5 n m m m
    //        PermuteElements<1, 0, 0, 0>(Rows[3]),
    //        //6 h h h g
    //        PermuteElements<3, 3, 3, 2>(Rows[1]),
    //        //7 j i i i
    //        PermuteElements<1, 0, 0, 0>(Rows[2]),
    //        //8 o o n n
    //        PermuteElements<2, 2, 1, 1>(Rows[3]),
    //        //9 b a a a
    //        PermuteElements<1, 0, 0, 0>(Rows[0]),
    //        //10 c c b b
    //        PermuteElements<2, 2, 1, 1>(Rows[0]),
    //        //11 d d d c
    //        PermuteElements<3, 3, 3, 2>(Rows[0])
    //    };

    //    //kp - lo
    //    auto back = ElementwiseSub(ElementwiseMul(temp[1], temp[2]), ElementwiseMul(temp[4], temp[8]));
    //    Rows[0] = ElementwiseMul(back, temp[0]);
    //    Rows[1] = ElementwiseMul(back, temp[9]);

    //    //ln - jp
    //    back = ElementwiseSub(ElementwiseMul(temp[4], temp[5]), ElementwiseMul(temp[7], temp[2]));
    //    Rows[0] = ElementwiseAdd(Rows[0], ElementwiseMul(back, temp[3]));
    //    Rows[1] = ElementwiseAdd(Rows[1], ElementwiseMul(back, temp[10]));

    //    //jo - nk
    //    back = ElementwiseSub(ElementwiseMul(temp[7], temp[8]), ElementwiseMul(temp[1], temp[5]));
    //    Rows[0] = ElementwiseAdd(Rows[0], ElementwiseMul(back, temp[6]));
    //    Rows[1] = ElementwiseAdd(Rows[1], ElementwiseMul(back, temp[11]));
    //    /*back = temp[7] * temp[8] - temp[1] * temp[5];
    //    Rows[0] += back * temp[6];
    //    Rows[1] += back * temp[11];*/

    //    //bg - cf
    //    back = ElementwiseSub(ElementwiseMul(temp[9], temp[3]), ElementwiseMul(temp[10], temp[0]));
    //    Rows[2] = ElementwiseAdd(Rows[2], ElementwiseMul(back, temp[2]));
    //    Rows[3] = ElementwiseAdd(Rows[3], ElementwiseMul(back, temp[4]));
    //   /* back = temp[9] * temp[3] - temp[10] * temp[0];
    //    Rows[2] = back * temp[2];
    //    Rows[3] = back * temp[4];*/

    //    //ch - dg
    //    back = ElementwiseSub(ElementwiseMul(temp[10], temp[6]), ElementwiseMul(temp[11], temp[3]));
    //    Rows[2] = ElementwiseAdd(Rows[2], ElementwiseMul(back, temp[5]));
    //    Rows[3] = ElementwiseAdd(Rows[3], ElementwiseMul(back, temp[7]));
    //    /*back = temp[10] * temp[6] - temp[11] * temp[3];
    //    Rows[2] += back * temp[5];
    //    Rows[3] += back * temp[7];*/

    //    //df - bh
    //    back = ElementwiseSub(ElementwiseMul(temp[11], temp[0]), ElementwiseMul(temp[9], temp[6]));
    //    Rows[2] = ElementwiseAdd(Rows[2], ElementwiseMul(back, temp[8]));
    //    Rows[3] = ElementwiseAdd(Rows[3], ElementwiseMul(back, temp[1]));
    //    /*back = temp[11] * temp[0] - temp[9] * temp[6];
    //    Rows[2] += back * temp[8];
    //    Rows[3] += back * temp[1];*/

    //    Rows[0] = ElementwiseMul(Rows[0], sign1);
    //    Rows[1] = ElementwiseMul(Rows[1], sign2);
    //    Rows[2] = ElementwiseMul(Rows[2], sign1);
    //    Rows[3] = ElementwiseMul(Rows[3], sign2);

    //    //将四个 Row 的第一个 sp 取出。
    //    auto determinant = SumElements(ElementwiseMul(Rows[0], abcd));

    //    Transpose();

    //    MatrixProduct(*this *= (1.f / determinant);

    //    return determinant;
    //}

    //float Matrix::Determinant() const noexcept
    //{
    //    const Vector4 temp[] = {
    //        //1 k k j j
    //        PermuteElements<2, 2, 1, 1>(Rows[2]),
    //        //2 p p p o
    //        PermuteElements<3, 3, 3, 2>(Rows[3]),
    //        //4 l l l k
    //        PermuteElements<3, 3, 3, 2>(Rows[2]),
    //        //5 n m m m
    //        PermuteElements<1, 0, 0, 0>(Rows[3]),
    //        //7 j i i i
    //        PermuteElements<1, 0, 0, 0>(Rows[2]),
    //        //8 o o n n
    //        PermuteElements<2, 2, 1, 1>(Rows[3]),
    //    };

    //    auto back = temp[0] * temp[1] - temp[2] * temp[5];
    //    Vector4 row = back * PermuteElements<1, 0, 0, 0>(Rows[1]);

    //    //ln - jp
    //    back = temp[2] * temp[3] - temp[4] * temp[1];
    //    row += back * PermuteElements<2, 2, 1, 1>(Rows[1]);

    //    //jo - nk
    //    back = temp[4] * temp[5] - temp[0] * temp[3];
    //    row += back * PermuteElements<3, 3, 3, 2>(Rows[1]);

    //    const auto sign1 = Vector4{ 1.f, -1.f, 1.f, -1.f };
    //    return (row * sign1 * Rows[0]).SumElements();
    //}
}
