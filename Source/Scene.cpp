#include "Scene.hpp"

namespace Fancy::Graphics
{
    std::weak_ptr<Object> Scene::FindObject(std::string_view name) const
    {
        auto p = objects_.find(std::string{ name });
        if (p == objects_.end())
            return {};
        else
            return p->second;
    }
}