#pragma once

/*! \file functional.hpp
 *  \brief function composition
 *
 *  Utilities for combining functions
 */

//! function composition
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
