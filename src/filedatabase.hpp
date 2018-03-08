#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace FDB
{
    /*! \brief Enum of possible datatypes
     *
     *  Enum used internally and in serialization to describe
     *  what datatype a field has.
     */
    enum class ValType : uint8_t
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
    struct Field
    {
        ValType      type;
        union
        {
            int32_t     int_val;
            float       flt_val;
            std::string str_val;
            bool        bol_val;
            int64_t     i64_val;
        };

        ~Field();
        Field();
        Field(const Field&);
    };

    /*! \brief Structure holding one table entry
     *
     *  Structure providing a list of fields and thereby being
     *  a container for one 'row' in the 'table'.
     */
    struct Row
    {
        std::vector<Field>   fields;
    };

    /*! \brief List of actual row entries
     *
     *  Theoretically a linked-list of rows, this structure holds all
     *  rows for a particular primary key.
     */
    struct Slot
    {
        /* The rows in this slot */
        std::vector<Row> rows;

        /* Get all matching rows */
        std::vector<Row> query(std::function<bool(const Row&)> predicate);
    };

    /*! \brief Column description for a table
     *
     *  Describes a column in a table, providing a name
     *  and default data type.
     */
    struct Column
    {
        std::string name;
        ValType  type;
    };

    /*! \brief Database table, consisting of column headers and row slots
     *
     *  Structure providing a name, a list of column headers and a list
     *  of slots. The slot index is usually related to the first column
     *  which is treated as a primary key. Strings are hashed by an yet
     *  to be known method.
     */
    struct Table
    {
        std::string         name;
        std::vector<Column> columns;
        std::vector<Slot>   slots;

        /* Get the slot for the specified primary key hash */
        Slot& slot(int primary_key);
        /* Get the column with the specified name */
        std::pair<int,Column&> column(const std::string& name);
    };

    /*! \brief Database schema, consisting of several tables
     *
     *  An sequence of database tables sorted lexicalically
     *  by their table name.
     */
    struct Schema
    {
        /* Sorted vector of tables */
        std::vector<Table> tables;

        /* Query for a table with a particular name, may throw exception */
        Table& table(const std::string& name);
    };
}
