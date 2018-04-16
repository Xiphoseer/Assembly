#include "package.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

#include "reader.hpp"


using namespace assembly::common::io;

namespace assembly::package
{
    void read_from_file(std::string filename, package_info& info)
    {
        std::ifstream infile(filename);
        if (!infile.is_open())
        {
            std::cerr << "Error loading: " << filename << std::endl;
            return;
        }

        infile.seekg(-8, infile.end);

        uint32_t dataAddr, unknown;
        read_uint32(infile, dataAddr);
        read_uint32(infile, unknown);

        infile.seekg(dataAddr, infile.beg);
        read_from_stream(infile, info);

        infile.close();
    }

    void read_package_info_entry(std::istream& i_strm, package_info_entry& entry)
    {
        read_uint32(i_strm, entry.crc);
        read_int32(i_strm, entry.iLeft);
        read_int32(i_strm, entry.iRight);
        read_int32(i_strm, entry.iUncompressedSize);
        read_buffer(i_strm, entry.chkUncompressed, 36);
        read_int32(i_strm, entry.iCompressedSize);
        read_buffer(i_strm, entry.chkCompressed, 36);
        read_uint32(i_strm, entry.uDataAddress);
        read_uint32(i_strm, entry.bCompressed);
    }

    void read_from_stream(std::istream& input, package_info& info)
    {
        uint32_t count = get_uint32(input);
        info.files.reserve(count);

        for (int i = 0; i < count; i++)
        {
            package_info_entry entry;
            read_package_info_entry(input, entry);
            info.files.push_back(entry);
        }
    }

    int write_package_info_entry(std::ostream& strm, const package_info_entry& entry)
    {
        write_uint32(strm, entry.crc);
        write_int32(strm, entry.iLeft);
        write_int32(strm, entry.iRight);
        write_int32(strm, entry.iUncompressedSize);
        write_buffer(strm, entry.chkUncompressed, 36);
        write_int32(strm, entry.iCompressedSize);
        write_buffer(strm, entry.chkCompressed, 36);
        write_uint32(strm, entry.uDataAddress);
        write_uint32(strm, entry.bCompressed);
    }

    void write_to_stream(std::ostream& output, const package_info& info)
    {
        write_vector_lu32<package_info_entry, write_package_info_entry>(output, info.files);
    }

    package_ptr::package_ptr(package_info* package, int32_t index) : package(package)
    {
        this->entry = package->files.at(index);
    }

    package_ptr::package_ptr(package_info* package) : package(package)
    {
        int32_t size = package->files.size();
        if (size > 0)
        {
            this->entry = package->files.at(size / 2);
        }
    }

    package_ptr::package_ptr() : package(nullptr)
    {
        this->entry.iLeft = -1;
        this->entry.iRight = -1;
    }

    bool package_ptr::valid()
    {
        return (this->package != nullptr) && (this->package->files.size() > 0);
    }

    uint32_t package_ptr::crc()
    {
        return this->entry.crc;
    }

    package_ptr package_ptr::left()
    {
        int32_t index = this->entry.iLeft;
        return (index == -1) ? package_ptr() : package_ptr(this->package, index);
    }

    package_ptr package_ptr::right()
    {
        int32_t index = this->entry.iRight;
        return (index == -1) ? package_ptr() : package_ptr(this->package, index);
    }

    void package_ptr::chkUncompressed(char* buffer)
    {
        strcpy(buffer, this->entry.chkUncompressed);
    }

    void package_ptr::chkCompressed(char* buffer)
    {
        strcpy(buffer, this->entry.chkCompressed);
    }

    uint32_t package_ptr::uncompressedSize()
    {
        return this->entry.iUncompressedSize;
    }

    uint32_t package_ptr::compressedSize()
    {
        return this->entry.iCompressedSize;
    }

    uint32_t package_ptr::dataAddress()
    {
        return this->entry.uDataAddress;
    }

    bool package_ptr::compressed()
    {
        // TODO: is the compressed the same as the data in pki?
        return (this->entry.bCompressed & 0xFF) != 0;
    }

    package_ptr find_by_crc(package_info* package, uint32_t crc)
    {
        package_ptr ptr(package);
        while (ptr.valid() && ptr.crc() != crc)
        {
            ptr = (crc > ptr.crc()) ? ptr.right() : ptr.left();
        }
        return ptr;
    }
}
