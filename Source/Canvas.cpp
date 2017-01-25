#include "Canvas.hpp"
#include <utility>
#include <algorithm>

namespace Fancy::Graphics
{
    Canvas::Canvas(std::uint32_t width, std::uint32_t height)
        : mainTexture_(width * height),
        width_{ width }, height_{ height }
    {}

    void Canvas::PutPixel(std::uint32_t x, std::uint32_t y, Math::Color c)
    {
        mainTexture_[y * width_ + x] = c;
    }

    std::uint32_t Canvas::GetPixelCount() const noexcept
    {
        return static_cast<std::uint32_t>(mainTexture_.size());
    }

    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    void Canvas::DrawLine(Math::Uint2 x, Math::Uint2 y, Math::Color c)
    {
        auto x0 = x.X, y0 = x.Y, x1 = y.X, y1 = y.Y;

        if (x0 == x1)
        {
            auto current = y0;
            while (true)
            {
                PutPixel(x0, current, c);
                if (current == y1)
                    break;
            }
        }
        else if (y0 == y1)
        {
            auto current = x0;
            while (true)
            {
                PutPixel(current, y0, c);
                if (current == x1)
                    break;
            }
        }
        else
        {
            const auto deltaX = x1 > x0 ? x1 - x0 : x0 - x1;
            const auto deltaY = y1 > y0 ? y1 - y0 : y0 - y1;

            std::int32_t epsilon = 0;
            if (deltaX > deltaY)
            {
                if (x0 > x1)
                {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                }
                const std::int32_t sign = y0 < y1 ? 1 : -1;
                for(; x0 <= x1; ++x0)
                {
                    PutPixel(x0, y0, c);
                    if (sign * 2 * (epsilon + deltaY) < sign * deltaX)
                    {
                        epsilon += deltaY;
                    }
                    else
                    {
                        y0 += sign;
                        epsilon = epsilon + deltaY - sign * deltaX;
                    }
                }
            }
            else
            {
                if (y0 > y1)
                {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                }
                const std::int32_t sign = x0 < x1 ? 1 : -1;
                for (; y0 <= y1; ++y0)
                {
                    PutPixel(x0, y0, c);
                    if (sign * 2 * (epsilon + deltaX) < sign * deltaY)
                    {
                        epsilon += deltaX;
                    }
                    else
                    {
                        x0 += sign;
                        epsilon = epsilon + deltaX - sign * deltaY;
                    }
                }
            }
        }
    }

    gsl::span<const Math::Color> Canvas::GetBitmap() const noexcept
    {
        return mainTexture_;
    }

    void Canvas::Resize(std::uint32_t newWidth, std::uint32_t newHeight)
    {
        mainTexture_.resize(newWidth * newHeight);
        width_ = newWidth;
        height_ = newHeight;
    }

    void Canvas::Clear(Math::Color color)
    {
        std::fill(mainTexture_.begin(), mainTexture_.end(), color);
    }
}
