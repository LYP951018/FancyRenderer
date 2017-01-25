#include "DXMathAdapters.hpp"
#include <Vectors.hpp>
#include <Matrix.hpp>
#include <Math.hpp>
#include <Quaternion.hpp>
#include <catch.hpp>
#include <DirectXMath.h>
#include <tuple>
#include <array>
#include <functional>

TEST_CASE("Simd")
{
    using namespace Fancy::Simd;

    auto i = MakeInts(-1, -2, -3, -4);

    CHECK(First(i) == -1);
    CHECK(Second(i) == -2);
    CHECK(Third(i) == -3);
    CHECK(Fourth(i) == -4);

    CHECK(Msbs(AsFloats(i)) == 0xF);

    Floats f = MakeFloats(1.f, 2.f, 3.f, 4.f);

    CHECK(Approx{ First(f) } == 1.f);
    CHECK(Approx{ Second(f) } == 2.f);
    CHECK(Approx{ Third(f) } == 3.f);
    CHECK(Approx{ Fourth(f) } == 4.f);

    CHECK(Msbs(Equal(f, MakeFloats(1.f, 2.f, 3.f, 4.f))) == 0xF);

    f = MakeFloats(1.f);

    CHECK(Approx{ First(f) } == 1.f);
    CHECK(Approx{ Second(f) } == 1.f);
    CHECK(Approx{ Third(f) } == 1.f);
    CHECK(Approx{ Fourth(f) } == 1.f);

    f = SetFirst(f, 2.f);
    CHECK(Approx{ First(f) } == 2.f);
    f = SetSecond(f, 3.f);
    CHECK(Approx{ Second(f) } == 3.f);
    f = SetThird(f, 4.f);
    CHECK(Approx{ Third(f) } == 4.f);
    f = SetFourth(f, 5.f);
    CHECK(Approx{ Fourth(f) } == 5.f);
}


TEST_CASE("Vectors")
{
    SECTION("Vector4<float>")
    {
        using namespace Fancy::Math;

        Vector4<float> vec1 = { 1.f, 2.f, 3.f, 4.f };
        

        CHECK(Approx{ vec1.X() } == 1.f);
        CHECK(Approx{ vec1.Y() } == 2.f);
        CHECK(Approx{ vec1.Z() } == 3.f);
        CHECK(Approx{ vec1.W() } == 4.f);

        Vector4<float> vec2 = { 3.f, 4.f, 5.f, 0.f };

        CHECK(Approx{ Dot3(vec1, vec2) } == (1.f * 3.f + 2.f * 4.f + 3.f * 5.f));

        auto vec3 = vec1;
        CHECK(Equal4(vec1, vec3));

        vec1.SetW(0.f);
        CHECK(Equal3(vec3, vec1));

        vec1.SetX(2.f);
        CHECK(!Equal3(vec3, vec1));

        vec1 = vec3;

        const auto length = std::sqrt(1.f * 1.f + 2.f * 2.f + 3.f * 3.f);
        CHECK(Approx{ Length3(vec1) } == length);

        vec1 = Normalize3(vec1);
        auto result = Float3{1.f / length, 2.f / length, 3.f / length};

        CHECK(Approx{ vec1.X() } == result.X);
        CHECK(Approx{ vec1.Y() } == result.Y);
        CHECK(Approx{ vec1.Z() } == result.Z);

        auto resultVec = MakePoint3D(result);

        CHECK(Approx{ resultVec.X() } == result.X);
        CHECK(Approx{ resultVec.Y() } == result.Y);
        CHECK(Approx{ resultVec.Z() } == result.Z);

        CHECK(NearEqual3(vec1, resultVec));
    }
}

TEST_CASE("Matrix")
{
    using namespace Fancy::Math;

    Matrix m{
        { 0.f, 4.f, 0.f, -3.f },
        { 1.f, 1.f, 5.f, 2.f },
        { 1.f, -2.f, 0.f, 6.f },
        { 3.f, 0.f, 0.f, 1.f }
    };

   /* CHECK(Vec4(m[0]) == Vector4{ 0.f, 4.f, 0.f, -3.f });
    CHECK(Vec4(m[1]) == Vector4{ 1.f, 1.f, 5.f, 2.f });
    CHECK(IsVec4Equal(m[2], Vector4{ 1.f, -2.f, 0.f, 6.f }));
    CHECK(IsVec4Equal(m[3], Vector4{ 3.f, 0.f, 0.f, 1.f }));
*/
    //CHECK(Approx{ -250.f } == m.Determinant());
    CHECK(m == m);

    m = Compose(m, m);
    auto result = Matrix{
        -5.f, 4.f, 20.f, 5.f,
        12.f, -5.f, 5.f, 31.f,
        16.f, 2.f, -10.f, -1.f,
        3.f, 12.f, 0.f, -8.f
    };
    CHECK( m == result);

    //m = Inverse(m);
    
    CHECK(ToFancyMatrix(DirectX::XMMatrixPerspectiveFovRH(kPi / 2.f, 1.f, 1.f, 1000.f)) == 
        Matrix::PerpspectiveFromFov(kPi / 2.f, 1.f, 1.f, 1000.f));
}



//TEST_CASE("Quaternion")
//{
//    using namespace Fancy::Math;
//    using namespace DirectX;
//    Quaternion q{ 1.0f, 0.f, 0.f, 0.f };
//    const auto m = GetRotationMatrix(q);
//    const auto dxVec = XMVectorSet(1.0f, 0.f, 0.f, 0.f);
//    const auto m2 = XMMatrixRotationQuaternion(dxVec);
//    CHECK(m2 == m);
//    const auto q2 = Quaternion::FromRotationMatrix(m);
//    CHECK(IsVec4Equal(q2.ToVector4(), q.ToVector4()));
//}