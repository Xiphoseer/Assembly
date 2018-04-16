#include "zone_io.hpp"
#include "reader.hpp"

#include <fstream>
#include <iostream>

namespace assembly::zone::io
{
    using namespace assembly::common;
    using namespace assembly::common::io;

    int read_scene_ref(std::istream& strm, scene_ref& scene, uint32_t version)
    {
        read_lu8_string(strm, scene.filename);
        read_uint32(strm, scene.id);
        read_uint32(strm, scene.is_audio);
        read_lu8_string(strm, scene.name);
        strm.ignore(3);
    }

    int read_trans_info(std::istream& strm, scene_transition_info& info, uint32_t version)
    {
        read_uint64(strm, info.scene_id);
        read_vector3f(strm, info.position);
    }

    int read_scene_trans(std::istream& strm, scene_transition& trans, uint32_t version)
    {
        if (version < 0x25) read_lu8_string(strm, trans.name);
        uint8_t loop_times = (version <= 0x21 || version >= 0x27) ? 2 : 5;

        for (uint8_t i = 0; i < loop_times; i++)
        {
            scene_transition_info info;
            read_trans_info(strm, info, version);
            trans.points.push_back(info);
        }
    }

    int read_from_stream(std::istream& strm, zone_file& zone)
    {
        read_uint32(strm, zone.version);
        if (zone.version >= 0x24) read_uint32(strm, zone.revision);

        read_uint32(strm, zone.world_id);
        if (zone.version >= 0x26) read_position(strm, zone.spawnpoint);

        uint32_t scene_count = (zone.version < 0x25) ? get_uint8(strm) : get_uint32(strm);
        for (int32_t i = 0; i < scene_count; i++)
        {
            scene_ref scene;
            read_scene_ref(strm, scene, zone.version);
            zone.scenes.push_back(scene);
        }

        read_lu8_string(strm, zone.something);
        read_lu8_string(strm, zone.map_filename);
        read_lu8_string(strm, zone.map_name);
        read_lu8_string(strm, zone.map_description);

        if (zone.version >= 0x20)
        {
            uint32_t transition_count = get_uint32(strm);
            for (uint32_t i = 0; i < transition_count; i++)
            {
                scene_transition trans;
                read_scene_trans(strm, trans, zone.version);
                zone.transitions.push_back(trans);
            }
        }

        if (zone.version >= 0x23)
        {
            uint32_t path_data_len = get_uint32(strm);
            zone.path_data.resize(path_data_len);
            strm.read((char*) &zone.path_data[0], path_data_len);
        }
    }

    int read_from_file(const std::string& filename, zone_file& zone)
    {
        std::ifstream infile(filename);
        if (infile.is_open())
        {
            read_from_stream(infile, zone);
            infile.close();
            return 0;
        }
        else
        {
            std::cerr << "Failed to load '" << filename << "' as zone!" << std::endl;
            return 1;
        }
    }
}

