#pragma once

#include "Color.hpp"
#include "Floats.hpp"
#include <vector>
#include <gsl/span>

namespace Fancy::Graphics
{
    class Canvas
    {
    public:
        Canvas(std::uint32_t width, std::uint32_t height)
            : mainTexture_(width * height),
            width_{width}, height_{height}
        {}

        Canvas() = default;

        void PutPixel(std::uint32_t x, std::uint32_t y, Math::Color c)
        {
            mainTexture_[y * width_ + x] = c;
        }

        std::uint32_t GetPixelCount() const noexcept
        {
            return width_ * height_;
        }

        void DrawLine(Math::Uint2 x, Math::Uint2 y, Math::Color c);
        void FillTriangle(Math::Float2 x, Math::Float2 y, Math::Float2 z, Math::Color c);

        DefGetter(GetWidth, std::uint32_t, width_);
        DefGetter(GetHeight, std::uint32_t, height_);

        gsl::span<const Math::Color> GetBitmap() const noexcept
        {
            return mainTexture_;
        }

        void Resize(std::uint32_t newWidth, std::uint32_t newHeight);
        void Clear(Math::Color color);

    private:
        std::vector<Math::Color> mainTexture_;
        std::uint32_t width_, height_;
    };
}