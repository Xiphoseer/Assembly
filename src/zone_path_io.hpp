#pragma once

/*! \file zone_path_io.hpp
 *  \brief Reading/Writing zone path data.
 *
 *  Utilities for reading and writing the path data within a
 *  zone file.
 */

#include "zone_path.hpp"


/*! \brief Reading/Writing zone path data.
 *
 *  Utilities for reading and writing the path data within a
 *  zone file.
 */
namespace assembly::zone::path::io
{
    /*! \brief Read from stream
     *
     *  Read path data from a stream.
     */
    int read_from_stream(std::istream&, zone_file_path_data&);

    /*! \brief Read from vector
     *
     *  Read path data from a byte vector.
     */
    int read_from_vector(const std::vector<char>&, zone_file_path_data&);
}
