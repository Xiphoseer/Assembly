#pragma once

/*! \file namevalue_io.hpp
 *  \brief Reading/Writing of typed name-value stores
 *
 *  Utilities for reading and writing name_value_store objects.
 */

#include "namevalue.hpp"

/*! \brief Reading/Writing typed name-value stores (LDF/LwoNameValue)
 *
 *  This namespace contains utility functions resposable for loading
 *  LDF/LwoNameValue formatted data from different serialization formats
 *  into a \link assembly::namevalue::name_value_store \endlink.
 */
namespace assembly::namevalue::io
{
    /*! \brief Read a store from a stringstream.
     *
     *  Read LDF data from a stringstream, and store them in the provided store. LDF data
     *  is a sequence of KEY=T:VAL strings, seperated by a delimiter, where T is an
     *  integer according to \link assembly::namevalue::value_type \endlink and VAL is
     *  a value string of the corresponding type. For more details on this format, refer
     *  to https://lu-docs.readthedocs.io/en/latest/intro.html#appendix-a-lego-data-format-and-data-type-ids
     *
     *  \param i_strm Input stream.
     *  \param store  Store to be filled.
     *  \param delim  The delimiter between entries (e.g. '\\n').
     *
     *  \return A status code.
     */
    int read_from_stream(std::istream& i_strm, name_value_store& store, char delim);

    /*! \brief Read a store from a stream
     *
     *  Read LDF data from a stream, and store them in the provided store. LDF data
     *  is a sequence of key and data strings, where the data is of the form T:VAL,
     *  where T represents the type of VAL is supposed to be interpreted.
     */
    int read_from_pair_stream(std::istream& i_strm, name_value_store& store);

    //! Read a store from a string
    int read_from_string(const std::string& str, name_value_store& store, char delim);

    //! Read a store from a utff16 string
    int read_from_u16string(const std::u16string& str, name_value_store& store, char delim);
}
