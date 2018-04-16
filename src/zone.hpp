#pragma once

/*! \file zone.hpp
 *  \brief Zone manipulation
 *
 *  Utilities to edit a zone / game world
 */

#include <string>
#include <vector>

#include "types.hpp"


//! Worlds within the game
namespace assembly::zone
{
    using namespace assembly::common;

    //! Reference to a scene within the zone
    struct scene_ref
    {
        //! Filename for the zone, relative to the luz path
        std::string filename;
        //! Globally unique zone id
        uint32_t id;
        //! Likely scene type; 0 = general, 1 = audio exist
        uint32_t is_audio;
        //! Name of the scene, never displayed to the user
        std::string name;
    };

    //! Point belonging to a specific transition
    struct scene_transition_info
    {
        //! ID of the scene transitioned to
        uint64_t scene_id;
        //! Position of the transition point
        vector3f position;
    };

    //! Data for a transition between scenes
    struct scene_transition
    {
        //! Name of the Transition
        std::string name;
        //! Vector of transition points
        std::vector<scene_transition_info> points;
    };

    /*! \brief File data for a zone (world)
     *
     *  This is a datastructure that represents all data extracted
     *  from a .luz file as found in the client files.
     */
    struct zone_file
    {
        //! Version of the file format
        uint32_t version;
        //! Likely revision counter for this file
        uint32_t revision;
        //! The zone id
        uint32_t world_id;
        //! The spawnpoint for this world
        position spawnpoint;
        //! A list of scenes
        std::vector<scene_ref> scenes;

        //! Unknown
        std::string something;
        //! The filename (.raw) of the height/map data
        std::string map_filename;
        //! Name of the map
        std::string map_name;
        //! Description of the map
        std::string map_description;

        //! A list of scene transitions
        std::vector<scene_transition> transitions;
        //! Binary path data
        std::vector<char> path_data;
    };
}
