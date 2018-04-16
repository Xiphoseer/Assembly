#pragma once

/*! \file scene.hpp
 *  \brief Scenes/Levels in the world
 *
 *  datastructures for manipulating scene data
 */

#include "types.hpp"

#include <string>
#include <vector>


//! Regions within a world
namespace assembly::scene
{
    using namespace assembly::common;

    enum class chunk_type : uint32_t
    {
        chnk_1000 = 1000,
        chnk_2000 = 2000,
        chnk_2001 = 2001,
        chnk_2002 = 2002
    };

    struct chunk_1000
    {
        uint32_t version;
        uint32_t value_1;
        uint32_t value_2;
        uint32_t value_3;
        uint32_t value_4;
    };

    struct chunk_2000_id
    {
        uint32_t id;
        float value_1;
        float value_2;
    };

    struct chunk_2000
    {
        std::vector<float> floats;
        std::vector<chunk_2000_id> ids;

        std::string sky_file_1;
        std::string sky_file_2;
        std::string sky_file_3;
        std::string sky_file_4;
        std::string sky_file_5;
        std::string sky_file_6;
    };

    struct chunk_2001_object
    {
        uint64_t id;
        int32_t lot;
        uint32_t value_1;
        uint32_t value_2;
        position pos;
        float scale;
        std::u16string settings;
        uint32_t value_3;
    };

    struct chunk_2001
    {
        std::vector<chunk_2001_object> objects;
    };

    struct chunk_2002
    {

    };

    struct chunk
    {
        chunk_type type;
        union
        {
            chunk_1000 v1000;
            chunk_2000 v2000;
            chunk_2001 v2001;
            chunk_2002 v2002;
        };

        chunk(chunk_type t);
        chunk(const chunk& c);
        ~chunk();
    };

    //! A region within the world, containing of objects
    struct scene_file
    {
        std::vector<chunk> chunks;
    };
}
