#pragma once

namespace logic
{
    template<typename T>
    std::function<bool(T)> lor(std::function<bool(T)> predA, std::function<bool(T)> predB)
    {
        return [predA, predB](T in){ return predA(in) || predB(in); };
    }

    template<typename T>
    std::function<bool(T)> land(std::function<bool(T)> predA, std::function<bool(T)> predB)
    {
        return [predA, predB](T in){ return predA(in) && predB(in); };
    }
}
