#pragma once

/*! \file manifest.hpp
 *  \brief managing manifest files
 *
 *  management of manifest files
 */

#include <string>
#include <vector>
#include <istream>


//! Manifest containing file lists
namespace assembly::manifest
{
    //! An entry within the manifest
    struct manifest_entry
    {
        std::string path;
        uint32_t sizeA;
        std::string checkA;
        uint32_t sizeB;
        std::string checkB;
        std::string checkC;
    };

    //! A manifest (.txt) file
    struct manifest_file
    {
        uint32_t fileVersion;
        std::string checksum;
        std::string version;
        std::vector<manifest_entry> files;

        manifest_entry              find(std::string path);
        std::vector<manifest_entry> query(const char* filter);
    };

    //! Load a manifest from a file
    int read_from_file(const std::string& filename, manifest_file& manifest);
    //! Load a manifest from a string
    int read_from_stream(std::istream& file, manifest_file& manifest);
}
