#include "database.hpp"

#include <stdexcept>
#include <algorithm>
#include <iostream>


namespace assembly::database
{
    field::field() : type(value_type::NOTHING) {}

    field::field(const field& field) : type(field.type)
    {
        //std::cout << "Copy" << (int) field.type << std::endl;
        switch(type)
        {
            case value_type::BOOLEAN:
            case value_type::INTEGER:  int_val = field.int_val; break;
            case value_type::FLOAT:    flt_val = field.flt_val; break;
            case value_type::BIGINT:   i64_val = field.i64_val; break;
            case value_type::VARCHAR:
            case value_type::TEXT:
                new(&str_val) std::string();
                str_val = field.str_val; break;
        }
        //std::cout << "Done" << std::endl;
    }

    field::~field()
    {
        using string = std::string;

        switch(type)
        {
            case value_type::VARCHAR:
            case value_type::TEXT:     str_val.~string();
        }
    }

    /*! \brief filters this slot using a given predicate
     *
     */
    std::vector<row> slot::query(std::function<bool(const row&)> predicate)
    {
        std::vector<row> result;
        for (row row : rows)
        {
            bool pred = predicate(row);
            if (pred) result.push_back(row);
        }
        return result;
    }

    /*! \brief Get the column with the specified name
     *
     */
    std::pair<int,const column&> table::column_def(const std::string& str) const
    {
        for (int i = 0; i < columns.size(); i++)
        {
            if (columns.at(i).name == str)
            {
                return std::pair<int,const column&>(i, columns.at(i));
            }
        }
        throw std::runtime_error("column not found");
    }

    /*! \brief Get the column selector
     *
     *  Returns a function which, given a row, returns the field
     *  corresponding to the given name from that row
     */
    std::function<const field&(const row&)> table::column_sel(const std::string& str) const
    {
        int i = this->column_def(str).first;
        return [i](const row& r) -> const field& { return r.fields.at(i); };
    }

    /*! \brief Get the slot for the specified key hash
     *
     *  Return a reference to the slot corresponding to
     *  the hash value, essentialy using modulo slot length.
     */
    slot& table::at(int primary_key)
    {
        return slots.at(primary_key % slots.size());
    }

    /*!
     * Const Version
     */
    const slot& table::at(int primary_key) const
    {
        return slots.at(primary_key % slots.size());
    }

    struct table_comp
    {
        bool operator() (const std::string& str, const table& tbl) { return str < tbl.name; }
        bool operator() (const table& tbl, const std::string& str) { return tbl.name < str; }
    };

    /*! \brief Get the table for a specified name
     *
     *  Use binary search to find the table with that name.
     */
    table& schema::at(const std::string& name)
    {
        std::vector<table>::iterator it = std::lower_bound(tables.begin(), tables.end(), name, table_comp{});
        if (it != tables.end() && it->name == name)
        {
            return *it;
        }
        throw std::runtime_error("table not found");
    }

    /*! \brief Get the table for a specified name
     *
     *  Use binary search to find the table with that name.
     */
    const table& schema::at(const std::string& name) const
    {
        std::vector<table>::const_iterator it = std::lower_bound(tables.begin(), tables.end(), name, table_comp{});
        if (it != tables.end() && it->name == name)
        {
            return *it;
        }
        throw std::runtime_error("table not found");
    }
}
