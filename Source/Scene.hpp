#pragma once

#include "Object.hpp"
#include "Camera.hpp"
#include <unordered_map>
#include <memory>

namespace Fancy::Graphics
{
    class Scene
    {
    public:
        std::weak_ptr<Object> FindObject(std::string_view name) const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Object>> objects_;
        Camera mainCamera_;
    };
}