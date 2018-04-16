#include "catalog.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>


namespace assembly::catalog
{
    int read_from_file(const std::string& filename, catalog_file& catalog)
    {
        std::ifstream infile(filename);
        if (infile.is_open())
        {
            read_from_stream(infile, catalog);
            infile.close();
            return 0;
        }
        else
        {
            std::cerr << "Failed to load '" << filename << "' as catalog!" << std::endl;
            return 1;
        }
    }

    int read_from_stream(std::istream& input, catalog_file& catalog)
    {
        /* Clean up the fields */
        catalog.pack_files.clear();
        catalog.files.clear();

        /* Load the version */
        input.read((char*) &(catalog.version), 4);

        /* Load the list of pack files */
        uint32_t pk_file_count;
        input.read((char*) &pk_file_count, 4);
        catalog.pack_files.reserve(pk_file_count);

        for (int i = 0; i < pk_file_count; i++)
        {
            uint32_t len;
            input.read((char*) &len, 4);

            char buf[len];
            input.read(buf, len);

            std::string name(buf, len);
            catalog.pack_files.push_back(name);
        }

        /* Load the list of packed files */
        uint32_t filesCount;
        input.read((char*) &filesCount, 4);
        catalog.files.reserve(filesCount);

        for (int i = 0; i < filesCount; i++)
        {
            catalog_entry entry;
            input.read((char*) &entry.crc, 4);
            input.read((char*) &entry.left, 4);
            input.read((char*) &entry.right, 4);
            input.read((char*) &entry.pack, 4);
            input.read((char*) &entry.data, 4);

            catalog.files.push_back(entry);
        }
    }

    /*
     * Returns the pack-index for a given pack file name
     */
    int catalog_file::get_pack_index(std::string pack_name)
    {
        /* Sanitize input */
        std::replace(pack_name.begin(), pack_name.end(), '/', '\\');

        for (int i = 0; i < this->pack_files.size(); i++)
        {
            if (pack_files.at(i) == pack_name) return i;
        }
        return -1;
    }

    catalog_ptr::catalog_ptr(catalog_file* file)
    {
        if (file->files.size() > 0)
        {
            int32_t root = file->files.size() / 2;
            this->file = file;
            this->entry = file->files.at(root);
        }
        else
        {
            this->file = nullptr;
        }
    }

    catalog_ptr::catalog_ptr(catalog_file* file, int32_t index) : file(file)
    {
        this->entry = file->files.at(index);
    }

    catalog_ptr::catalog_ptr()
    {
        this->file = nullptr;
        this->entry = catalog_entry();
    }

    bool catalog_ptr::valid()
    {
        return this->file != nullptr;
    }

    catalog_ptr catalog_ptr::left()
    {
        int32_t id = this->entry.left;
        return (id == -1) ? catalog_ptr() : catalog_ptr(this->file, id);
    }

    catalog_ptr catalog_ptr::right()
    {
        int32_t id = this->entry.right;
        return (id == -1) ? catalog_ptr() : catalog_ptr(this->file, id);
    }

    std::string catalog_ptr::pack()
    {
        return this->file->pack_files.at(this->entry.pack);
    }

    catalog_ptr find_by_crc(catalog_file* file, uint32_t crc)
    {
        catalog_ptr ptr(file);
        while (ptr.valid() && ptr.crc() != crc)
        {
            ptr = (crc > ptr.crc()) ? ptr.right() : ptr.left();
        }
        return ptr;
    }
}
