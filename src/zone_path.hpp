#pragma once

/*! \file zone_path.hpp
 *  \brief Datastructures for path data
 *
 *  Datastructures for the path data within zone files.
 */

#include <vector>
#include <string>

#include "types.hpp"
#include "namevalue.hpp"


/*! \brief Path data within a zone file
 *
 *  Datastructures representing the path data within the zone files.
 */
namespace assembly::zone::path
{
    using namespace assembly::common;
    using namespace assembly::namevalue;

    //! PathBehavior enum
    enum class path_behavior_enum
    {
        loop = 0,
        bounce = 1,
        once = 2
    };

    enum class path_type
    {
        movement = 0,
        moving_platform = 1,
        property = 2,
        camera = 3,
        spawner = 4,
        showcase = 5,
        race = 6,
        rail = 7
    };

    struct v3f_config
    {
        vector3f pos;
        name_value_store config;
    };

    //! Data for movement path
    struct path_data_movement
    {
        //! Waypoints
        std::vector<v3f_config> waypoints;
    };

    //! Data for moving platform path point
    struct moving_platform_point
    {
        //! Position
        position pos;
        //! Lock player until next waypoint
        bool lock_player;
        //! Speed
        float speed;
        //! Wait
        float wait;
        //! Depart Sound
        std::u16string depart_sound;
        //! Arrive Sound
        std::u16string arrive_sound;
    };

    //! Data for moving platform path
    struct path_data_moving_platform
    {
        //! unknown
        uint8_t value_1;

        //! unknown
        std::u16string travel_sound;

        //! Waypoints
        std::vector<moving_platform_point> waypoints;
    };

    //! Time unit
    enum class time_unit : int32_t
    {
        forever = 0,
        seconds = 1,
        minutes = 2,
        hours   = 3,
        days    = 4,
        weeks   = 5,
        months  = 6,
        years   = 7
    };

    //! Property-related achievements
    enum class property_achievement : int32_t
    {
        none             = 0,
        builder          = 1,
        craftsman        = 2,
        senior_builder   = 3,
        journeyman       = 4,
        master_builder   = 5,
        architect        = 6,
        senior_architect = 7,
        master_architect = 8,
        visionary        = 9,
        exemplar         = 10
    };

    //! Data for property path
    struct path_data_property
    {
        int32_t value_1;
        int32_t price;
        int32_t rental_time;
        uint64_t zone;
        std::u16string name;
        std::u16string description;
        int32_t value_2;
        int32_t clone_limit;
        float reputation_multiplier;
        time_unit rental_time_unit;
        property_achievement req_achievement;
        vector3f player_coord;
        float max_building_height;

        std::vector<vector3f> waypoints;
    };

    //! Data for camera path point
    struct camera_path_entry
    {
        vector3f pos;
        float value_1;
        float value_2;
        float value_3;
        float value_4;
        float time;
        float value_5;
        float tension;
        float continuity;
        float bias;
    };

    //! Data for camera path
    struct path_data_camera
    {
        std::u16string next_path;
        uint8_t value_1;
        std::vector<camera_path_entry> waypoints;
    };

    struct spawner_path_entry
    {
        position pos;
        name_value_store config;
    };

    struct path_data_spawner
    {
        uint32_t spawned_lot;
        uint32_t respawn_time;
        int32_t  max_to_spawn;
        uint32_t number_to_maintain;
        int64_t  object_id;
        bool     activate_on_load;

        std::vector<spawner_path_entry> waypoints;
    };

    struct path_data_showcase
    {
        std::vector<vector3f> waypoints;
    };

    struct race_path_entry
    {
        position pos;
        uint8_t value_1;
        uint8_t value_2;
        float value_3;
        float value_4;
        float value_5;
    };

    struct path_data_race
    {
        std::vector<race_path_entry> waypoints;
    };

    struct rail_path_entry
    {
        vector3f pos;
        float value_1;
        float value_2;
        float value_3;
        float value_4;
        float value_5;
        name_value_store config;
    };

    struct path_data_rail
    {
        std::vector<rail_path_entry> waypoints;
    };


    /*! \brief Path-Data entry in zone file
     *
     *  One entry in the path data within a zone file
     */
    struct path_data_entry
    {
        //! FileFormat version
        uint32_t path_version;
        //! Name
        std::u16string name;
        //! behavior?
        path_behavior_enum behavior;
        //! unknown
        uint32_t value_1;

        //! Path type
        path_type type;

        union
        {
            path_data_movement        d_movement;
            path_data_moving_platform d_moving_platform;
            path_data_property        d_property;
            path_data_camera          d_camera;
            path_data_spawner         d_spawner;
            path_data_showcase        d_showcase;
            path_data_race            d_race;
            path_data_rail            d_rail;
        };

        path_data_entry(const path_type&);

        path_data_entry(const path_data_entry&);

        path_data_entry(const path_data_entry&&);

        ~path_data_entry();
    };

    /*! \brief Path data from zone file
     *
     *  The path data found at the end of some zone files
     */
    struct zone_file_path_data
    {
        uint32_t value_1;
        std::vector<path_data_entry> paths;
    };
}
