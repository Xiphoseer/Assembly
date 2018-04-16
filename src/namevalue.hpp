#pragma once
/*! \file namevalue.hpp
 *  \brief Typed key-value store
 *
 *  A typed key value store used throughout the game, with multiple formats
 *  existing to serialize it (string/binary).
 */
#include <string>
#include <vector>
#include <map>


//! Typed name-value store
namespace assembly::namevalue
{
    /*! \brief integer type information
     *
     *  Definitions for the integer values corresponding to the
     *  value options.
     */
    enum class value_type
    {
        string_v = 0, //!< string value
        int32_v = 1,  //!< 32bit signed integer

        float_v = 3,  //!< 32bit floating point number
        // loc_size_v = 4;
        uint32_v = 5, //!< 32bit unsigned integer

        bool_v = 7,   //!< boolean value
        int64_v = 8,  //!< 64bit signed integer
        id64_v = 9,   //!< 64bit signed integer for IDs

        binary_v = 13 //!< binary data
    };

    /*! \brief typed value
     *
     *  value to hold a value of exactly one type
     */
    struct any_value
    {
        //! Type of the value
        value_type type;

        //! Different values
        union
        {
            std::string       v_string; //!< String value
            int32_t           v_int32;  //!< 32bit integer value
            float             v_float;  //!< Float value
            uint32_t          v_uint32; //!< 32bit unsigned integer value
            bool              v_bool;   //!< Boolean value
            int64_t           v_int64;  //!< 64bit integer value
            std::vector<char> v_blob;   //!< Binary value
        };

        //! Constructor from type
        any_value(const value_type& type);

        //! Copy-Constructor
        any_value(const any_value& val);

        //! Move-Constructor
        any_value(any_value&& val);

        //! Assignment Operator
        any_value& operator=(const any_value& ref)
        {
            return *(new(this) any_value(ref));
        }

        //! Destructor
        ~any_value();

        std::string       get_string() { return v_string; } //!< Getter for string values
        int32_t           get_int32()  { return v_int32; }  //!< Getter for int32_t values
        float             get_float()  { return v_float; }  //!< Getter for float values
        uint32_t          get_uint32() { return v_uint32; } //!< Getter for uint32_t values
        bool              get_bool()   { return v_bool; }   //!< Getter for boolean values
        int64_t           get_int64()  { return v_int64; }  //!< Getter for int64_t values
        std::vector<char> get_blob()   { return v_blob; }   //!< Getter for binary values
    };

    /*! \brief Typed name-value store
     *
     *  This std::map specialization is responsible for representing
     *  a LDF/LwoNameValue datastructure in memory. Such a datastructure
     *  consists of string keys associated with values of different types,
     *  according to \link assembly::namevalue::value_type \endlink.
     */
    typedef std::map<std::string, any_value> name_value_store;
}
