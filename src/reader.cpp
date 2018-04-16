#include "reader.hpp"

namespace assembly::common
{
    namespace io
    {
        /* Reader functions */
        int read_uint8(std::istream& strm, uint8_t& data)
        {
            strm.read((char*) &data, 1);
            check_eof(strm);
        }

        int read_uint32(std::istream& strm, uint32_t& data)
        {
            strm.read((char*) &data, 4);
            check_eof(strm);
        }

        int read_uint64(std::istream& strm, uint64_t& data)
        {
            strm.read((char*) &data, 8);
            check_eof(strm);
        }

        int read_int32(std::istream& strm, int32_t& data)
        {
            strm.read((char*) &data, 4);
            check_eof(strm);
        }

        int read_int64(std::istream& strm, int64_t& data)
        {
            strm.read((char*) &data, 8);
            check_eof(strm);
        }

        int read_float(std::istream& strm, float& data)
        {
            strm.read((char*) &data, 4);
            check_eof(strm);
        }

        int read_string(std::istream& strm, std::string& str, uint32_t n)
        {
            str.resize(n);
            strm.read(&str[0], n);
            check_eof(strm);
        }

        int read_u16string(std::istream& strm, std::u16string& str, uint32_t n)
        {
            str.resize(n);
            strm.read((char*) &str[0], n * 2);
            check_eof(strm);
        }

        int read_lu8_string(std::istream& strm, std::string& str)
        {
            read_string(strm, str, get_uint8(strm));
        }

        int read_lu32_string(std::istream& strm, std::string& str)
        {
            read_string(strm, str, get_uint32(strm));
        }

        int read_lu8_u16string(std::istream& strm, std::u16string& str)
        {
            read_u16string(strm, str, get_uint8(strm));
        }

        int read_lu32_u16string(std::istream& strm, std::u16string& str)
        {
            read_u16string(strm, str, get_uint32(strm));
        }

        int read_buffer(std::istream& strm, char* data, uint32_t n)
        {
            strm.read(data, n);
            check_eof(strm);
        }

        /* Getter functions */
        uint8_t get_uint8(std::istream& strm)
        {
            uint8_t val;
            read_uint8(strm, val);
            return val;
        }

        uint32_t get_uint32(std::istream& strm)
        {
            uint32_t val;
            read_uint32(strm, val);
            return val;
        }

        float get_float(std::istream& strm)
        {
            float val;
            read_float(strm, val);
            return val;
        }

        std::string get_string(std::istream& strm, uint32_t n)
        {
            std::string val;
            read_string(strm, val, n);
            return val;
        }

        std::u16string get_lu8_u16string(std::istream& strm)
        {
            std::u16string str;
            read_lu8_u16string(strm, str);
            return str;
        }

        /* Struct reader functions */
        int read_position(std::istream& strm, position& pos)
        {
            read_vector3f(strm, pos.pos);
            read_quaternion(strm, pos.rot);
        }

        int read_position_wxyz(std::istream& strm, position& pos)
        {
            read_vector3f(strm, pos.pos);
            read_quaternion_wxyz(strm, pos.rot);
        }

        int read_vector3f(std::istream& strm, vector3f& vec)
        {
            read_float(strm, vec.x);
            read_float(strm, vec.y);
            read_float(strm, vec.z);
        }

        int read_quaternion_base(std::istream& strm, quaternion& rot)
        {
            read_float(strm, rot.x);
            read_float(strm, rot.y);
            read_float(strm, rot.z);
        }

        int read_quaternion(std::istream& strm, quaternion& rot)
        {
            read_quaternion_base(strm, rot);
            read_float(strm, rot.w);
        }

        int read_quaternion_wxyz(std::istream& strm, quaternion& rot)
        {
            read_float(strm, rot.w);
            read_quaternion_base(strm, rot);
        }

        int write_uint8(std::ostream& strm, uint8_t data)
        {
            strm.write((char*) &data, 1);
        }

        int write_uint16(std::ostream& strm, uint16_t data)
        {
            strm.write((char*) &data, 2);
        }

        int write_uint32(std::ostream& strm, uint32_t data)
        {
            strm.write((char*) &data, 4);
        }

        int write_uint64(std::ostream& strm, uint64_t data)
        {
            strm.write((char*) &data, 8);
        }

        int write_int8(std::ostream& strm, int8_t data)
        {
            strm.write((char*) &data, 1);
        }

        int write_int16(std::ostream& strm, int16_t data)
        {
            strm.write((char*) &data, 2);
        }

        int write_int32(std::ostream& strm, int32_t data)
        {
            strm.write((char*) &data, 4);
        }

        int write_int64(std::ostream& strm, int64_t data)
        {
            strm.write((char*) &data, 8);
        }

        int write_buffer(std::ostream& strm, const char* data, uint32_t n)
        {
            strm.write(data, n);
        }
    }
}
