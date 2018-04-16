#pragma once
#include "zone.hpp"

/*! \file zone_io.hpp
 *  \brief Zone file (.luz) reading
 *
 *  Utilities for reading zone files
 */

//! Reading/Writing zone files
namespace assembly::zone::io
{
    //! Read a zone from an input stream
    int read_from_stream(std::istream& strm, zone_file& zone);
    //! Read a zone from a file with the given path
    int read_from_file(const std::string& filename, zone_file& zone);
}
