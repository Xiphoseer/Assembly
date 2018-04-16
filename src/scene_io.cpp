#include "scene_io.hpp"
#include "reader.hpp"

#include <iostream>
#include <fstream>


namespace assembly::scene::io
{
    using namespace assembly::common;
    using namespace assembly::common::io;

    void read_chnk_1000(std::istream& i_strm, chunk_1000& chnk, uint32_t& version)
    {
        read_uint32(i_strm, chnk.version);
        read_uint32(i_strm, chnk.value_1);
        read_uint32(i_strm, chnk.value_2);
        read_uint32(i_strm, chnk.value_3);
        read_uint32(i_strm, chnk.value_4);

        version = chnk.version;
    }

    chunk_2000_id get_chunk_2000_id(std::istream& i_strm)
    {
        chunk_2000_id id;
        read_uint32(i_strm, id.id);
        read_float(i_strm, id.value_1);
        read_float(i_strm, id.value_2);
        return id;
    }

    void read_chnk_2000(std::istream& i_strm, chunk_2000& chnk, uint32_t& version)
    {
        uint32_t data_size = get_uint32(i_strm);
        uint32_t sky_data_addr = get_uint32(i_strm);
        uint32_t other_data_addr = get_uint32(i_strm);

        uint32_t float_count = (data_size - 8) / 4;

        for (uint32_t i = 0; i < float_count; i++)
        {
            float f = get_float(i_strm);
            chnk.floats.push_back(f);
        }

        uint32_t id_count = get_uint32(i_strm);

        for (int i = 0; i < id_count; i++)
        {
            chnk.ids.push_back(get_chunk_2000_id(i_strm));
        }

        i_strm.seekg(sky_data_addr);

        read_lu32_string(i_strm, chnk.sky_file_1);
        read_lu32_string(i_strm, chnk.sky_file_2);
        read_lu32_string(i_strm, chnk.sky_file_3);
        read_lu32_string(i_strm, chnk.sky_file_4);
        read_lu32_string(i_strm, chnk.sky_file_5);
        read_lu32_string(i_strm, chnk.sky_file_6);

        i_strm.seekg(other_data_addr);
    }

    chunk_2001_object get_chunk_2001_object(std::istream& i_strm, uint32_t version)
    {
        chunk_2001_object obj;

        read_uint64(i_strm, obj.id);
        read_int32(i_strm, obj.lot);
        if (version >= 0x26)
            read_uint32(i_strm, obj.value_1);
        if (version >= 0x20)
            read_uint32(i_strm, obj.value_2);
        read_position_wxyz(i_strm, obj.pos);
        read_float(i_strm, obj.scale);
        read_lu32_u16string(i_strm, obj.settings);
        read_uint32(i_strm, obj.value_3);

        return obj;
    }

    void read_chnk_2001(std::istream& i_strm, chunk_2001& chnk, uint32_t version)
    {
        uint32_t obj_count = get_uint32(i_strm);

        for (int i = 0; i < obj_count; i++)
        {
            chnk.objects.push_back(get_chunk_2001_object(i_strm, version));
        }
    }

    void read_chnk_2002(std::istream& i_strm, chunk_2002& chnk, uint32_t& version)
    {

    }

    int read_from_stream(std::istream& i_strm, scene_file& lvl)
    {
        uint32_t version;

        while (!i_strm.eof())
        {
            uint32_t chnk_start = i_strm.tellg();

            char buf[5];
            buf[5] = 0;

            i_strm.read(buf, 4);
            if (std::string(buf) == "CHNK" && !i_strm.eof())
            {
                chunk chnk(static_cast<chunk_type>(get_uint32(i_strm)));

                std::cout << get<uint16_t>(i_strm) << ":";
                std::cout << get<uint16_t>(i_strm) << std::endl;
                //i_strm.ignore(4);
                uint32_t chnk_len = get_uint32(i_strm);
                uint32_t chnk_off = get_uint32(i_strm);

                uint32_t target = chnk_start + chnk_len;
                i_strm.seekg(chnk_off);

                switch (chnk.type)
                {
                    case chunk_type::chnk_1000: read_chnk_1000(i_strm, chnk.v1000, version); break;
                    case chunk_type::chnk_2000: read_chnk_2000(i_strm, chnk.v2000, version); break;
                    case chunk_type::chnk_2001: read_chnk_2001(i_strm, chnk.v2001, version); break;
                    case chunk_type::chnk_2002: read_chnk_2002(i_strm, chnk.v2002, version); break;
                }

                lvl.chunks.push_back(chnk);
                i_strm.seekg(target);
            }
            else
            {
                // TODO : Old File
                break;
            }
        }
    }

    int read_from_file(const std::string& s_file, scene_file& lvl)
    {
        std::ifstream f_in(s_file);
        if (f_in.is_open())
        {
            read_from_stream(f_in, lvl);
            f_in.close();
            return 0;
        }
        else
        {
            std::cerr << "Failed to load '" << s_file << "' as scene!" << std::endl;
            return 1;
        }
    }
}
