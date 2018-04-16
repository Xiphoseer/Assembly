#pragma once
#include "database.hpp"

/*! \file fdb_query.hpp
 *  \brief FileDataBase querying tools
 *
 *  Utilities for searching through the file database
 */

#include <string>
#include <vector>
#include <cstddef>
#include <iterator>


//! Query utilities for the database
namespace assembly::database::query
{
    struct like
    {
        bool require_empty = false;
        std::string require_exact = "";
        std::string require_start = "";
        std::string require_end = "";
        std::vector<std::string> require_seq;

        like(const std::vector<std::string>& parts);
        like(const std::string& query);

        bool operator()(const field& field);
    };

    struct int_eq
    {
        int require_exact;

        int_eq(int require_exact);
        bool operator()(const field& field);
    };

    template<class S, class T,
        typename slot_iter = typename std::vector<S>::iterator,
        typename row_iter = typename std::vector<T>::iterator>

    struct iterator_base
    {
        typedef T         value_type;
        typedef ptrdiff_t difference_type;
        typedef T*        pointer;
        typedef T&        reference;
        typedef std::forward_iterator_tag iterator_category;

        slot_iter slot_it;
        slot_iter slot_end;

        row_iter row_it;
        row_iter row_end;

        void _find_next()
        {
            while (this->row_it == this->row_end)
            {
                this->slot_it++;
                if (this->slot_it != this->slot_end)
                {
                    this->row_it = this->slot_it->rows.begin();
                    this->row_end = this->slot_it->rows.end();
                }
                else
                {
                    return;
                }
            }
        };

        iterator_base<S,T,slot_iter,row_iter> (slot_iter slot_it, slot_iter slot_end):
            slot_it(slot_it),slot_end(slot_end)
        {
            this->row_it = this->slot_it->rows.begin();
            this->row_end = this->slot_it->rows.end();
            this->_find_next();
        };

        iterator_base<S,T,slot_iter,row_iter>(
            slot_iter slot_it, slot_iter slot_end,
            row_iter row_it, row_iter row_end):
            slot_it(slot_it), slot_end(slot_end),
            row_it(row_it), row_end(row_end)
        {};

        iterator_base<S,T,slot_iter,row_iter>(const iterator_base<S,T,slot_iter,row_iter>& o):
            iterator_base<S,T,slot_iter,row_iter>(
                slot_iter(o.slot_it), slot_iter(o.slot_end),
                row_iter(o.row_it), row_iter(o.row_end)) {};

        iterator_base<S,T,slot_iter,row_iter>& operator++()
        {
            if (this->slot_it == this->slot_end) return *this;
            {
                this->row_it++;
                this->_find_next();
            }
        };

        iterator_base<S,T,slot_iter,row_iter> operator+=(int i)
        {
            while (this->slot_it != this->slot_end)
            {
                this++;
            }
        };

        iterator_base<S,T,slot_iter,row_iter> operator+(int i)
        {
            return iterator_base<S,T,slot_iter,row_iter>(*this) += i;
        };

        T* operator->()
        {
            return *(this->row_it);
        };

        T& operator*()
        {
            return *(this->row_it);
        };

        operator bool()
        {
            return this->slot_it != this->slot_end;
        };
    };

    typedef iterator_base<slot, row> iterator;
    typedef iterator_base<const slot, const row,
        std::vector<slot>::const_iterator, std::vector<row>::const_iterator> const_iterator;

    const_iterator for_table(const table& tbl)
    {
        return const_iterator(tbl.slots.begin(), tbl.slots.end());
    }

    iterator for_table(table& tbl)
    {
        return iterator(tbl.slots.begin(), tbl.slots.end());
    }
}
