#pragma once
#include "filedatabase.hpp"

#include <string>
#include <vector>
#include <cstddef>
#include <iterator>

namespace FDB::Query
{
    struct Like
    {
        bool require_empty = false;
        std::string require_exact = "";
        std::string require_start = "";
        std::string require_end = "";
        std::vector<std::string> require_seq;

        Like(const std::vector<std::string>& parts);
        Like(const std::string& query);

        bool operator()(const FDB::Field& field);
    };

    template<class S, class T,
        typename slot_iter = typename std::vector<S>::iterator,
        typename row_iter = typename std::vector<T>::iterator>

    struct IteratorBase
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

        void _findNext()
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

        IteratorBase<S,T,slot_iter,row_iter> (slot_iter slot_it, slot_iter slot_end):
            slot_it(slot_it),slot_end(slot_end)
        {
            this->row_it = this->slot_it->rows.begin();
            this->row_end = this->slot_it->rows.end();
            this->_findNext();
        };

        IteratorBase<S,T,slot_iter,row_iter>(
            slot_iter slot_it, slot_iter slot_end,
            row_iter row_it, row_iter row_end):
            slot_it(slot_it), slot_end(slot_end),
            row_it(row_it), row_end(row_end)
        {};

        IteratorBase<S,T,slot_iter,row_iter>(const IteratorBase<S,T,slot_iter,row_iter>& o):
            IteratorBase<S,T,slot_iter,row_iter>(
                slot_iter(o.slot_it), slot_iter(o.slot_end),
                row_iter(o.row_it), row_iter(o.row_end)) {};

        IteratorBase<S,T,slot_iter,row_iter>& operator++()
        {
            if (this->slot_it == this->slot_end) return *this;
            {
                this->row_it++;
                this->_findNext();
            }
        };

        IteratorBase<S,T,slot_iter,row_iter> operator+=(int i)
        {
            while (this->slot_it != this->slot_end)
            {
                this++;
            }
        };

        IteratorBase<S,T,slot_iter,row_iter> operator+(int i)
        {
            return IteratorBase<S,T,slot_iter,row_iter>(*this) += i;
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

    typedef IteratorBase<Slot, Row> Iterator;
    typedef IteratorBase<const Slot, const Row,
        std::vector<Slot>::const_iterator, std::vector<Row>::const_iterator> ConstIterator;

    ConstIterator for_table(const Table& tbl)
    {
        return ConstIterator(tbl.slots.begin(), tbl.slots.end());
    }

    Iterator for_table(Table& tbl)
    {
        return Iterator(tbl.slots.begin(), tbl.slots.end());
    }
}
