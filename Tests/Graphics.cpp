#include <Canvas.hpp>
#include <catch.hpp>

TEST_CASE("Canvas")
{
    using namespace Fancy::Graphics;

    Canvas canvas{ 300, 300 };
    canvas.DrawLine({ 0, 0 }, { 3, 3 }, Fancy::Math::Color{ UINT32_MAX });
}