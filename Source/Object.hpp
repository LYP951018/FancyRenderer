#pragma once

#include "Vertex.hpp"
#include <vector>
#include <string>
#include <string_view>

namespace Fancy::Graphics
{
    class Object
    {
    public:
        Math::Float3 Position;
        Math::Float3 Rotation;
        Math::Float3 Scale;

        Object(std::string_view name);

        virtual void OnUpdate();

    private:
        const std::string name_;
        std::vector<Vertex> vertexes_;
        std::vector<std::uint16_t> indices_;
    };
}