#include <Math/Vector.hpp>
#include <Math/Matrix.hpp>
#include <DirectXMath.h>
#include <iostream>

int main()
{
    using namespace Fancy::Math;

    //Vector4 vec1{ 1.f, 2.f, 3.f, 4.f };
    //Vector4 vec2{ 1.2f, 2.f, 3.f, 4.f };
    //std::cout << vec1 * vec2;
    //std::cout << /*X(vec1) << Y(vec1); */vec1.X() << vec1.Y();//X(vec1);
    //vec1.SetX(3.f);
    //vec1.SetY(5.f);
    //std::cout << /*X(vec1) << Y(vec1); */vec1.X() << vec1.Y();//X(vec1);

    Matrix m{
        { 0.f, 4.f, 0.f, -3.f },
        { 1.f, 1.f, 5.f, 2.f },
        { 1.f, -2.f, 0.f, 6.f },
        { 3.f, 0.f, 0.f, 1.f }
    };

    m.Inverse();
    /*DirectX::XMMATRIX m2 = {
        0.f, 4.f, 0.f, -3.f, 1.f, 1.f, 5.f, 2.f, 1.f, -2.f, 0.f, 6.f, 3.f, 0.f, 0.f, 1.f
    };

    DirectX::XMMatrixInverse({}, m2);*/
}