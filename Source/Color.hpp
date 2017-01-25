#pragma once

#include "Vectors.hpp"
#include <cstdint>

namespace Fancy::Math
{
    //using Color = Vector4<float>;

    struct Color
    {
        union
        {
            struct 
            {
                std::uint8_t B, G, R, A;
            };
            std::uint32_t C;
        };

        Color() = default;

        Color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept
            : R{r}, G{g}, B{b}, A{a}
        {}

        explicit Color(std::uint32_t color) noexcept
            : C{color}
        {}
    };

    DefConstInlineVar(Vector4<float>, Blue, { 0.f, 0.f, 1.f, 1.f });
    DefConstInlineVar(Vector4<float>, Red, { 1.f, 0.f, 0.f, 1.f });
    DefConstInlineVar(Vector4<float>, Green, { 0.f, 1.f, 0.f, 1.f });
    DefConstInlineVar(Vector4<float>, White, { 1.f, 1.f, 1.f, 1.f });
    DefConstInlineVar(Vector4<float>, Black, { 0.f, 0.f, 0.f, 1.f });
}