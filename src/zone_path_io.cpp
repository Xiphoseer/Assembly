#include "zone_path_io.hpp"
#include "namevalue_io.hpp"
#include "reader.hpp"
#include "utf.hpp"

#include <sstream>
#include <iostream>


namespace assembly::zone::path::io
{
    using namespace assembly::common::io;
    using namespace assembly::namevalue;

    inline int read_config(std::istream& i_strm, name_value_store& data)
    {
        return assembly::namevalue::io::read_from_pair_stream(i_strm, data);
    }

    int read_v3f_config(std::istream& i_strm, v3f_config& data)
    {
        read_vector3f(i_strm, data.pos);
        read_config(i_strm, data.config);
    }

    int read_path_movement(std::istream& i_strm, path_data_movement& data, uint32_t path_version)
    {
        read_vector<v3f_config, uint32_t, read_v3f_config>(i_strm, data.waypoints);
    }

    int read_moving_platform_point(std::istream& i_strm, moving_platform_point& pt, uint32_t path_version)
    {
        read_position_wxyz(i_strm, pt.pos);
        read_bool<uint8_t>(i_strm, pt.lock_player);
        read_float(i_strm, pt.speed);
        read_float(i_strm, pt.wait);

        if (path_version >= 13)
        {
            read_varstring<uint8_t, char16_t>(i_strm, pt.depart_sound);
            read_varstring<uint8_t, char16_t>(i_strm, pt.arrive_sound);
        }
    }

    int read_path_moving_platform(std::istream& i_strm, path_data_moving_platform& data, uint32_t path_version)
    {
        //std::cout << path_version << std::endl;
        /*if (path_version >= 18) read_uint8(i_strm, data.value_1);
        std::cout << ((int32_t) data.value_1) << std::endl;*/

        if (path_version >= 13)
        {
            //read_varstring<uint8_t, char16_t>(i_strm, data.travel_sound);
            uint32_t len = get_uint8(i_strm);
            //std::cout << "LEN: " << len << std::endl;

            if (len > 1)
            {
                read_string<char16_t>(i_strm, data.travel_sound, len);
                //std::cout << utf::to_utf8(data.travel_sound) << std::endl;

                /*for (int j = 0; j < 30; j++)
                {
                    std::cout << ((uint32_t) get_uint8(i_strm)) << std::endl;
                }*/
            }

            /*for (uint8_t i = 0; i < len; i++)
            {
                std::cout << get_uint32(i_strm) << std::endl;

                /**/

                //read_varstring<uint8_t, char16_t>(i_strm, data.travel_sound);
                //std::cout << "DAT: " << utf::to_utf8(data.travel_sound) << std::endl;
            //}

            /*uint32_t cnt = get_uint32(i_strm);
            std::cout << ((int32_t) cnt) << std::endl;

            for (int i = 0; i < cnt; i++)
            {
                std::cout << get<float>(i_strm) << std::endl;
            }

            for (int i = 0; i < 30; i++)
            {
                std::cout << ((uint32_t) get_uint8(i_strm)) << std::endl;
            }
            //read_string<char>(i_strm, data.travel_sound, cnt);

            exit(0);*/
        }

        //uint32_t count = get_uint32(i_strm);
        //std::cout << count << std::endl;
        read_vector<moving_platform_point, uint32_t, uint32_t, read_moving_platform_point>(i_strm, data.waypoints, path_version);
    }

    int read_path_property(std::istream& i_strm, path_data_property& data, uint32_t path_version)
    {
        read_int32(i_strm, data.value_1);
        read_int32(i_strm, data.price);
        read_int32(i_strm, data.rental_time);
        read_uint64(i_strm, data.zone);
        read_varstring<uint8_t, char16_t>(i_strm, data.name);
        read_varstring<uint32_t, char16_t>(i_strm, data.description);
        read_int32(i_strm, data.value_2);
        read_int32(i_strm, data.clone_limit);
        read_float(i_strm, data.reputation_multiplier);
        read_enum<time_unit, int32_t>(i_strm, data.rental_time_unit);
        read_enum<property_achievement, int32_t>(i_strm, data.req_achievement);
        read_vector3f(i_strm, data.player_coord);
        read_float(i_strm, data.max_building_height);

        read_vector<vector3f, uint32_t, read_vector3f>(i_strm, data.waypoints);
    }

    int read_camera_path_entry(std::istream& i_strm, camera_path_entry& entry)
    {
        read_vector3f(i_strm, entry.pos);
        read_float(i_strm, entry.value_1);
        read_float(i_strm, entry.value_2);
        read_float(i_strm, entry.value_3);
        read_float(i_strm, entry.value_4);
        read_float(i_strm, entry.time);
        read_float(i_strm, entry.value_5);
        read_float(i_strm, entry.tension);
        read_float(i_strm, entry.continuity);
        read_float(i_strm, entry.bias);
    }

    int read_path_camera(std::istream& i_strm, path_data_camera& data, uint32_t path_version)
    {
        read_varstring<uint8_t, char16_t>(i_strm, data.next_path);
        if (path_version >= 14) read_uint8(i_strm, data.value_1);
        read_vector<camera_path_entry, uint32_t, read_camera_path_entry>(i_strm, data.waypoints);
    }

    int read_spawner_path_entry(std::istream& i_strm, spawner_path_entry& entry)
    {
        read_position_wxyz(i_strm, entry.pos);
        read_config(i_strm, entry.config);
    }

    int read_path_spawner(std::istream& i_strm, path_data_spawner& data, uint32_t path_version)
    {
        read_uint32(i_strm, data.spawned_lot);
        read_uint32(i_strm, data.respawn_time);
        read_int32(i_strm, data.max_to_spawn);
        read_uint32(i_strm, data.number_to_maintain);
        read_int64(i_strm, data.object_id);
        read_bool<uint8_t>(i_strm, data.activate_on_load);

        read_vector<spawner_path_entry, uint32_t, read_spawner_path_entry>(i_strm, data.waypoints);
    }

    int read_path_showcase(std::istream& i_strm, path_data_showcase& data, uint32_t path_version)
    {
        read_vector<vector3f, uint32_t, read_vector3f>(i_strm, data.waypoints);
    }

    int read_race_path_entry(std::istream& i_strm, race_path_entry& entry)
    {
        read_position_wxyz(i_strm, entry.pos);
        read_uint8(i_strm, entry.value_1);
        read_uint8(i_strm, entry.value_2);
        read_float(i_strm, entry.value_3);
        read_float(i_strm, entry.value_4);
        read_float(i_strm, entry.value_5);
    }

    int read_path_race(std::istream& i_strm, path_data_race& data, uint32_t path_version)
    {
        read_vector<race_path_entry, uint32_t, read_race_path_entry>(i_strm, data.waypoints);
    }

    int read_rail_path_entry(std::istream& i_strm, rail_path_entry& entry, uint32_t path_version)
    {
        read_vector3f(i_strm, entry.pos);
        read<float>(i_strm, entry.value_1);
        read<float>(i_strm, entry.value_2);
        read<float>(i_strm, entry.value_3);
        read<float>(i_strm, entry.value_4);
        if (path_version >= 17)
        {
            read<float>(i_strm, entry.value_5);
        }
        read_config(i_strm, entry.config);
    }

    int read_path_rail(std::istream& i_strm, path_data_rail& data, uint32_t path_version)
    {
        read_vector<rail_path_entry, uint32_t, uint32_t, read_rail_path_entry>(i_strm, data.waypoints, path_version);
    }

    int read_path(std::istream& i_strm, std::vector<path_data_entry>& paths)
    {
        uint32_t path_version = get_uint32(i_strm);
        std::u16string name = get_varstring<uint8_t, char16_t>(i_strm);

        path_data_entry entry(get_enum<path_type, uint32_t>(i_strm));

        entry.path_version = path_version;
        entry.name = name;
        read_uint32(i_strm, entry.value_1);
        read_enum<path_behavior_enum, uint32_t>(i_strm, entry.behavior);

        switch (entry.type)
        {
            case path_type::movement:        read_path_movement        (i_strm, entry.d_movement,        path_version); break;
            case path_type::moving_platform: read_path_moving_platform (i_strm, entry.d_moving_platform, path_version); break;
            case path_type::property:        read_path_property        (i_strm, entry.d_property,        path_version); break;
            case path_type::camera:          read_path_camera          (i_strm, entry.d_camera,          path_version); break;
            case path_type::spawner:         read_path_spawner         (i_strm, entry.d_spawner,         path_version); break;
            case path_type::showcase:        read_path_showcase        (i_strm, entry.d_showcase,        path_version); break;
            case path_type::race:            read_path_race            (i_strm, entry.d_race,            path_version); break;
            case path_type::rail:            read_path_rail            (i_strm, entry.d_rail,            path_version); break;
        }

        paths.push_back(entry);
    }

    int read_from_stream(std::istream& i_strm, zone_file_path_data& paths)
    {
        read_uint32(i_strm, paths.value_1);
        read_vector_lu32<path_data_entry, read_path>(i_strm, paths.paths);
    }

    int read_from_vector(const std::vector<char>& vector, zone_file_path_data& paths)
    {
        std::stringstream strm(std::string(vector.begin(), vector.end()));
        return read_from_stream(strm, paths);
    }
}
