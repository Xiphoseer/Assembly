#pragma once

/*! \file database.hpp
 *  \brief FileDataBase for game content
 *
 *  Datastructures for the database which stores most game data.
 */

#include <vector>
#include <string>
#include <memory>
#include <functional>


//! Database for the game
namespace assembly::database
{
    /*! \brief Enum of possible datatypes
     *
     *  Enum used internally and in serialization to describe
     *  what datatype a field has.
     */
    enum class value_type : uint8_t
    {
        /* Used as null value */
        NOTHING = 0,
        /* Standard 32bit signed integer */
        INTEGER,
        /* Never used as far as we know */
        UNKNOWN1,
        /* Standard IEEE ... single precision float */
        FLOAT,
        /* Text or possibly binary blob */
        TEXT,
        /* Boolean: (int_val != 0) */
        BOOLEAN,
        /* Standard 64bit signed integer, possibly DATETIME too */
        BIGINT,
        /* Never used as far as we know */
        UNKNOWN2,
        /* Text or possibly binary blob */
        VARCHAR
    };

    /*! \brief Smallest unit, represents a value
     *
     *  A field structure which can hold a value of exactly one
     *  of the datatypes at all times.
     */
    struct field
    {
        value_type      type;
        union
        {
            int32_t     int_val;
            float       flt_val;
            std::string str_val;
            bool        bol_val;
            int64_t     i64_val;
        };

        ~field();
        field();
        field(const field&);
    };

    /*! \brief Structure holding one table entry
     *
     *  Structure providing a list of fields and thereby being
     *  a container for one 'row' in the 'table'.
     */
    struct row
    {
        std::vector<field>   fields;
    };

    /*! \brief List of actual row entries
     *
     *  Theoretically a linked-list of rows, this structure holds all
     *  rows for a particular primary key.
     */
    struct slot
    {
        /* The rows in this slot */
        std::vector<row> rows;

        /* Get all matching rows */
        std::vector<row> query(std::function<bool(const row&)> predicate);
    };

    /*! \brief column description for a table
     *
     *  Describes a column in a table, providing a name
     *  and default data type.
     */
    struct column
    {
        std::string name;
        value_type  type;
    };

    /*! \brief Database table, consisting of column headers and row slots
     *
     *  Structure providing a name, a list of column headers and a list
     *  of slots. The slot index is usually related to the first column
     *  which is treated as a primary key. Strings are hashed by an yet
     *  to be known method.
     */
    struct table
    {
        std::string         name;
        std::vector<column> columns;
        std::vector<slot>   slots;

        //! Get the slot for the specified primary key hash
        slot& at(int primary_key);
        //! Get the slot for the specified primary key hash
        const slot& at(int primary_key) const;

        //! Get the column with the specified name
        std::pair<int,const column&> column_def(const std::string& name) const;
        //! Get the selector for the specified field
        std::function<const field&(const row&)> column_sel(const std::string& name) const;
    };

    /*! \brief Database schema, consisting of several tables
     *
     *  An sequence of database tables sorted lexicalically
     *  by their table name.
     */
    struct schema
    {
        //! Sorted vector of tables
        std::vector<table> tables;

        //! Query for a table with a particular name, may throw exception
        table& at(const std::string& name);
        //! Const version
        const table& at(const std::string& name) const;
    };
}
