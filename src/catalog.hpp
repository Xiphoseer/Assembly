#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <istream>

/*! \file catalog.hpp
 *  \brief Catalog for packages
 *
 *  This file provides classes allowing to read and manipulate
 *  the catalog (.pki) file which is used to locate files
 *  within the game packages.
 */

//! The catalog for game packages
namespace assembly::catalog
{

    //! One entry within a catalog file
    struct catalog_entry
    {
        uint32_t crc;    //!< The CRC-32 value for the filename
        int32_t left;    //!< The left child of this node
        int32_t right;   //!< The right child of this node
        int32_t pack;    //!< The index of the pack that this catalog_entry contains
        uint32_t data;   //!< Additional data, possibly a category or gating id
    };

    /*! \brief A catalog file such as 'versions/primary.pki'
     *
     */
    struct catalog_file
    {
        std::vector<std::string> pack_files;
        std::vector<catalog_entry> files;
        uint32_t version;

        int get_pack_index(std::string pack_name);
    };

    //! Load a catalog from a file
    int read_from_file(const std::string& filename, catalog_file& catalog);
    //! Load a catalog from a stream
    int read_from_stream(std::istream& file, catalog_file& catalog);

    /*! \brief catalog reading helper
     *
     *  Helper class to make walking through the catalog
     *  easier by interpreting the entry values on the fly.
     */
    class catalog_ptr
    {
    private:
        catalog_entry entry;
        catalog_file* file;
        catalog_ptr(catalog_file* file, int32_t index);
        catalog_ptr();

    public:
        catalog_ptr(catalog_file* file);
        bool valid();

        //! Get the CRC value of the entry
        uint32_t crc() { return entry.crc; }
        //! Get the left child entry
        catalog_ptr left();
        //! Get the right child entry
        catalog_ptr right();
        //! Get the pack filename
        std::string pack();
        //! Get the pack index
        int32_t pack_id() { return entry.pack; }
        //! Get the data
        uint32_t data() { return entry.data; }
    };

    catalog_ptr find_by_crc(catalog_file* file, uint32_t crc);
}
