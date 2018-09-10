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
    struct table_header_entry
    {
        int32_t column_header_addr;
        int32_t row_top_header_addr;
    };

    struct schema_header
    {
        int32_t table_count;
        int32_t table_header_addr;
    };

    struct column_header
    {
        int32_t column_count;
        int32_t table_name_addr;
        int32_t column_data_addr;
    };

    struct column_data
    {
        int32_t data_type;
        int32_t name_addr;
    };

    struct row_top_header
    {
        int32_t slot_count;
        int32_t row_header_addr;
    };

    struct row_info
    {
        int32_t row_data_header_addr;
        int32_t next_row_addr;
    };

    struct row_data_header
    {
        int32_t column_count;
        int32_t row_data_addr;
    };

    struct row_data
    {
        int32_t type;
        int32_t value;
    };

    //! Read the database from a stream
    int read_from_stream(std::istream& strm, schema& schema, bool data = true);

    //! Read the database from a file
    int read_from_file(const std::string& filename, schema& schema, bool data = true);
}
