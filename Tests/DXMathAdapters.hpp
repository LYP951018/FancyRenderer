#pragma once

#include <DirectXMath.h>
#include <Matrix.hpp>
#include <Quaternion.hpp>

inline DirectX::XMVECTOR ToDXMathVector(Fancy::Math::Vector4<float> vec)
{
    return vec;
}

inline Fancy::Math::Vector4<float> ToFancyVector(DirectX::XMVECTOR vec)
{
    return vec;
}

inline bool operator== (DirectX::XMMATRIX dxMathMatrix, Fancy::Math::Matrix fancyMatrix)
{
    for (std::size_t i = 0; i < 4; ++i)
        if (!Equal4(ToFancyVector(dxMathMatrix.r[0]), fancyMatrix.Rows[0]))
            return false;
    return true;
}


inline DirectX::XMVECTOR ToDXMathVector(Fancy::Math::Quaternion q)
{
    return ToDXMathVector(q.GetFloats());
}

inline DirectX::XMMATRIX ToDXMathMatrix(Fancy::Math::Matrix m)
{
    return {
        m.Rows[0], m.Rows[1], m.Rows[2], m.Rows[3]
    };
}

inline Fancy::Math::Matrix ToFancyMatrix(DirectX::XMMATRIX m)
{
    return {
        m.r[0], m.r[1], m.r[2], m.r[3]
    };
}

inline std::ostream& operator<< (std::ostream& os, DirectX::XMMATRIX dxMatrix)
{
    return os << ToFancyMatrix(dxMatrix);
}