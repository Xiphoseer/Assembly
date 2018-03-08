#include "filedatabase.hpp"

#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace FDB
{
    Field::Field() : type(ValType::NOTHING) {}

    Field::Field(const Field& field) : type(field.type)
    {
        //std::cout << "Copy" << (int) field.type << std::endl;
        switch(type)
        {
            case ValType::BOOLEAN:
            case ValType::INTEGER:  int_val = field.int_val; break;
            case ValType::FLOAT:    flt_val = field.flt_val; break;
            case ValType::BIGINT:   i64_val = field.i64_val; break;
            case ValType::VARCHAR:
            case ValType::TEXT:
                new(&str_val) std::string();
                str_val = field.str_val; break;
        }
        //std::cout << "Done" << std::endl;
    }

    Field::~Field()
    {
        using string = std::string;

        switch(type)
        {
            case ValType::VARCHAR:
            case ValType::TEXT:     str_val.~string();
        }
    }

    /*! \brief filters this slot using a given predicate
     *
     */
    std::vector<Row> Slot::query(std::function<bool(const Row&)> predicate)
    {
        std::vector<Row> result;
        for (Row row : rows)
        {
            bool pred = predicate(row);
            if (pred) result.push_back(row);
        }
        return result;
    }

    /*! \brief Get the column with the specified name
     *
     */
    std::pair<int,Column&> Table::column(const std::string& str)
    {
        for (int i = 0; i < columns.size(); i++)
        {
            if (columns.at(i).name == str)
            {
                return std::pair<int,Column&>(i, columns.at(i));
            }
        }
        throw std::runtime_error("Column not found");
    }

    /*! \brief Get the slot for the specified key hash
     *
     *  Return a reference to the slot corresponding to
     *  the hash value, essentialy using modulo slot length.
     */
    Slot& Table::slot(int primary_key)
    {
        return slots.at(primary_key % slots.size());
    }

    /*! \brief Get the table for a specified name
     *
     *  Use binary search to find the table with that name.
     */
    Table& Schema::table(const std::string& name)
    {
        struct Comp
        {
            bool operator() (const std::string& str, const Table& tbl) { return str < tbl.name; }
            bool operator() (const Table& tbl, const std::string& str) { return tbl.name < str; }
        };

        std::vector<Table>::iterator it = std::lower_bound(tables.begin(), tables.end(), name, Comp{});
        if (it != tables.end() && it->name == name)
        {
            return *it;
        }
        throw std::runtime_error("Table not found");
    }
}
