#include "fdb_reader.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>


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

void read_int(std::istream& strm, int32_t& val)
{
    strm.read((char*) &val, 4);
}

table_header_entry read_table_header_entry(std::istream& strm)
{
    table_header_entry entry;
    read_int(strm, entry.column_header_addr);
    read_int(strm, entry.row_top_header_addr);
    return entry;
}

schema_header read_schema_header(std::istream& strm)
{
    schema_header header;
    read_int(strm, header.table_count);
    read_int(strm, header.table_header_addr);
    return header;
}

column_header read_column_header(std::istream& strm)
{
    column_header header;
    read_int(strm, header.column_count);
    read_int(strm, header.table_name_addr);
    read_int(strm, header.column_data_addr);
    return header;
}

column_data read_column_data(std::istream& strm)
{
    column_data data;
    read_int(strm, data.data_type);
    read_int(strm, data.name_addr);
    return data;
}

row_top_header read_row_top_header(std::istream& strm)
{
    row_top_header header;
    read_int(strm, header.slot_count);
    read_int(strm, header.row_header_addr);
    return header;
}

row_info read_row_info(std::istream& strm)
{
    row_info info;
    read_int(strm, info.row_data_header_addr);
    read_int(strm, info.next_row_addr);
    return info;
}

row_data_header read_row_data_header(std::istream& strm)
{
    row_data_header header;
    read_int(strm, header.column_count);
    read_int(strm, header.row_data_addr);
    return header;
}

row_data read_row_data(std::istream& strm)
{
    row_data data;
    read_int(strm, data.type);
    read_int(strm, data.value);
    return data;
}

namespace FDB
{
    int readField(std::istream& strm, Field& field, row_data data)
    {
        field.type = static_cast<ValType>(data.type);
        switch(field.type)
        {
            case ValType::INTEGER:
            case ValType::FLOAT:    // use the union for reinterpretation
            case ValType::BOOLEAN:

                field.int_val = data.value;
                break;

            case ValType::TEXT:
            case ValType::VARCHAR:

                // Initialize a string
                new(&field.str_val) std::string();

                strm.seekg(data.value);
                std::getline(strm, field.str_val, '\0');

                break;

            case ValType::UNKNOWN1:
            case ValType::UNKNOWN2:

                throw std::runtime_error("Unknown Type, please investigate");
        }
    }

    int readRowDataHeader(std::istream& strm, Row& row)
    {
        row_data_header header = read_row_data_header(strm);

        strm.seekg(header.row_data_addr);
        std::vector<row_data> data(header.column_count);
        row.fields.reserve(header.column_count);

        for (int i = 0; i < header.column_count; i++)
        {
            data.at(i) = read_row_data(strm);
            row.fields.push_back(Field());
        }

        for (int i = 0; i < header.column_count; i++)
        {
            readField(strm, row.fields.at(i), data.at(i));
        }
    }

    int readRowInfo(std::istream& strm, Slot& slot)
    {
        row_info info = read_row_info(strm);
        Row& row = *slot.rows.insert(slot.rows.end(), Row());

        strm.seekg(info.row_data_header_addr);
        readRowDataHeader(strm, row);

        if (info.next_row_addr != -1)
        {
            strm.seekg(info.next_row_addr);
            readRowInfo(strm, slot);
        }
    }

    int readRowTopHeader(std::istream& strm, Table& table)
    {
        row_top_header header = read_row_top_header(strm);

        strm.seekg(header.row_header_addr);
        std::vector<int32_t> slot_addrs(header.slot_count);
        table.slots.reserve(header.slot_count);

        for (int i = 0; i < header.slot_count; i++)
        {
            table.slots.push_back(Slot());
            int32_t row_info_addr;
            read_int(strm, row_info_addr);
            slot_addrs.at(i) = row_info_addr;
        }

        for (int i = 0; i < header.slot_count; i++)
        {
            if (slot_addrs.at(i) != -1)
            {
                strm.seekg(slot_addrs.at(i));
                readRowInfo(strm, table.slots.at(i));
            }
        }
    }

    int readColumnHeader(std::istream& strm, Table& table)
    {
        column_header header = read_column_header(strm);

        strm.seekg(header.table_name_addr);
        std::getline(strm, table.name, '\0');

        strm.seekg(header.column_data_addr);
        std::vector<column_data> col_data(header.column_count);
        table.columns.reserve(header.column_count);

        for (int i = 0; i < header.column_count; i++)
        {
            col_data.at(i) = read_column_data(strm);
            table.columns.push_back(Column());
        }

        for (int i = 0; i < header.column_count; i++)
        {
            table.columns.at(i).type = static_cast<ValType>(col_data.at(i).data_type);
            strm.seekg(col_data.at(i).name_addr);
            std::getline(strm, table.columns.at(i).name, '\0');
        }
    }

    int readFromStream(std::istream& strm, Schema& schema)
    {
        schema_header header = read_schema_header(strm);

        strm.seekg(header.table_header_addr);
        std::vector<table_header_entry> table_addrs(header.table_count);

        for (int i = 0; i < header.table_count; i++)
        {
            table_header_entry entry = read_table_header_entry(strm);
            table_addrs.at(i) = entry;

            schema.tables.push_back(Table());
        }

        for (int i = 0; i < header.table_count; i++)
        {
            strm.seekg(table_addrs.at(i).column_header_addr);
            readColumnHeader(strm, schema.tables.at(i));
            strm.seekg(table_addrs.at(i).row_top_header_addr);
            readRowTopHeader(strm, schema.tables.at(i));
        }
    }

    int readFromFile(const std::string& filename, Schema& schema)
    {
        std::ifstream file(filename);
        if (file.is_open())
        {
            int i = readFromStream(file, schema);
            file.close();
            return i;
        }
        else
        {
            std::cout << "Failed to load '" << filename << '"' << std::endl;
        }
        return 1;
    }
}
