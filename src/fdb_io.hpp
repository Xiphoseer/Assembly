#pragma once

/*! \file fdb_io.hpp
 *  \brief FileDataBase reading tools
 *
 *  Utilities for reading a database from file.
 */

#include "database.hpp"
#include <iostream>


namespace assembly::database::io
{
    //! Read the database from a stream
    int read_from_stream(std::istream& strm, schema& schema, bool data = true);

    //! Read the database from a file
    int read_from_file(const std::string& filename, schema& schema, bool data = true);
}
