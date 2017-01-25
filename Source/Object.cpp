#include "Object.hpp"

namespace Fancy::Graphics
{
    Object::Object(std::string_view name)
        : name_{name}
    {}

    void Object::OnUpdate()
    {
    }

    Object::~Object()
    {
    }
}
