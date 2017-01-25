#pragma once

#include <cstdint>

namespace Fancy::Math
{
    struct alignas(16) AlignedFloat2
    {
        float X, Y;
    };

    struct Float2
    {
        float X, Y;
    };

    struct Int2
    {
        std::int32_t X, Y;
    };

    struct Uint2
    {
        std::uint32_t X, Y;
    };

    struct alignas(16) AlignedFloat3
    {
        float X, Y, Z;
    };

    struct Float3
    {
        float X, Y, Z;
    };

    struct alignas(16) AlignedFloat4
    {
        float X, Y, Z, W;
    };

    struct Float4
    {
        float X, Y, Z, W;
    };

    struct alignas(16) AlignedFloat4x4
    {
        float M[4][4];
    };

    struct Float4x4
    {
        float M[4][4];
    };

    struct alignas(16) AlignedFloat4x3
    {
        float M[4][3];
    };

    struct Float4x3
    {
        float M[4][3];
    };
}