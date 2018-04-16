#pragma once
/*! \file types.hpp
 *  \brief Common datastructures
 *
 *  ...
 */

//! Common datastructures
namespace assembly::common
{
    struct vector3f
    {
        float x;
        float y;
        float z;

        static const vector3f ZERO;
    };

    inline bool operator== (const vector3f& a, const vector3f& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z;
    }

    inline bool operator!= (const vector3f& a, const vector3f& b)
    {
        return a.x != b.x || a.y != b.y || a.z != b.z;
    }

    struct quaternion
    {
        float x;
        float y;
        float z;
        float w;

        static const quaternion IDENTITY;
    };

    inline bool operator== (const quaternion& a, const quaternion& b)
    {
        return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    }

    inline bool operator!= (const quaternion& a, const quaternion& b)
    {
        return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
    }

    struct position
    {
        vector3f pos;
        quaternion rot;
    };

    inline bool operator== (const position& a, const position& b)
    {
        return a.pos == b.pos && a.rot == b.rot;
    }

    inline bool operator!= (const position& a, const position& b)
    {
        return a.pos != b.pos || a.rot != b.rot;
    }

}
