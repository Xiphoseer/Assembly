#pragma once

/*! \file package.hpp
 *  \brief Game file packages
 *
 *  The structures in this file represent one of the package
 *  files used to store the game data.
 */

#include <istream>
#include <ostream>
#include <vector>
#include <string>


//! The game file packages
namespace assembly::package
{
    struct package_info_entry
    {
        uint32_t crc;
        int32_t iLeft;
        int32_t iRight;
        int32_t iUncompressedSize;
        char chkUncompressed[36];
        int32_t iCompressedSize;
        char chkCompressed[36];
        uint32_t uDataAddress;
        uint32_t bCompressed;
    };

    struct package_info
    {
        std::vector<package_info_entry> files;
    };

    void read_from_file(std::string filename, package_info& info);
    void read_from_stream(std::istream& input, package_info& info);
    void write_to_stream(std::ostream& output, const package_info& info);

    class package_ptr
    {
        package_info* package;
        package_info_entry entry;
        package_ptr(package_info* package, int32_t index);
        package_ptr();
    public:

        package_ptr(package_info* package);
        bool valid();
        uint32_t crc();
        package_ptr left();
        package_ptr right();
        void chkCompressed(char* buffer);
        void chkUncompressed(char* buffer);
        uint32_t uncompressedSize();
        uint32_t compressedSize();
        uint32_t dataAddress();
        bool compressed();
    };

    package_ptr find_by_crc(package_info* package, uint32_t crc);
}
