#include "database.hpp"

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <sstream>

#include "utf.hpp"


namespace assembly::database
{
    const std::string value_type_name(const value_type& type)
    {
        switch(type)
        {
            case value_type::NOTHING: return "NOTHING";
            case value_type::BOOLEAN: return "BOOLEAN";
            case value_type::INTEGER: return "INTEGER";
            case value_type::FLOAT:   return "FLOAT";
            case value_type::BIGINT:  return "BIGINT";
            case value_type::VARCHAR: return "VARCHAR";
            case value_type::TEXT:    return "TEXT";

            case value_type::UNKNOWN1:    return "[UNKNOWN|1]";
            case value_type::UNKNOWN2:    return "[UNKNOWN|2]";
        }

        throw std::runtime_error("Can't get name for unknown type " + (uint8_t) type);
    }

    field::field() : type(value_type::NOTHING) {}

    field::field(const field& field) : type(field.type)
    {
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

    std::string field::to_string() const
    {
        switch(type)
        {
            case value_type::NOTHING: return "_null";
            case value_type::BOOLEAN: return this->int_val != 0 ? "_true" : "_false";
            case value_type::INTEGER: return std::to_string(this->int_val);
            case value_type::FLOAT:   return std::to_string(this->flt_val);
            case value_type::BIGINT:  return std::to_string(this->i64_val);
            case value_type::VARCHAR:
            case value_type::TEXT:    return utf::from_latin_1(this->str_val);
        }

        throw std::runtime_error("Can't get name for unknown type " + (uint8_t) type);
    }

    int32_t field::get_int() const
    {
      if (this->type == value_type::INTEGER) {
        return this->int_val;
      } else {
        throw std::runtime_error("Could not read " + value_type_name(this->type) + " field as integer");
      }
    }

    float field::get_float() const
    {
      if (this->type == value_type::FLOAT) {
        return this->flt_val;
      } else {
        throw std::runtime_error("Could not read " + value_type_name(this->type) + " field as float");
      }
    }

    std::string field::get_str() const
    {
      if (this->type == value_type::TEXT || this->type == value_type::VARCHAR) {
        return this->str_val;
      } else {
        throw std::runtime_error("Could not read " + value_type_name(this->type) + " field as string");
      }
    }

    std::string field::get_str(const std::string& default_str) const
    {
      if (this->type == value_type::TEXT || this->type == value_type::VARCHAR) {
        return this->str_val;
      } else if (this->type == value_type::NOTHING) {
        return default_str;
      } else {
        throw std::runtime_error("Could not read " + value_type_name(this->type) + " field as string");
      }
    }

    bool field::get_bool() const
    {
      if (this->type == value_type::BOOLEAN) {
        return this->bol_val;
      } else {
        throw std::runtime_error("Could not read " + value_type_name(this->type) + " field as boolean");
      }
    }

    int64_t field::get_int64() const
    {
      if (this->type == value_type::BIGINT) {
        return this->i64_val;
      } else {
        throw std::runtime_error("Could not read " + value_type_name(this->type) + " field as long integer");
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
        std::stringstream err_str;
        err_str << "column `" << this->name << "`.`" << str << "` not found";
        throw std::runtime_error(err_str.str());
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
