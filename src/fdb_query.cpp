#include "fdb_query.hpp"

#include <algorithm>

#include "database.hpp"
#include "stringutil.hpp"

namespace assembly::database
{
    namespace query
    {
        like::like (const std::vector<std::string>& parts)
        {
            if (parts.size() == 0)
            {
                this->require_empty = true;
            }
            else if (parts.size() == 1)
            {
                this->require_empty = parts.at(0).empty();
                this->require_exact = parts.at(0);
            }
            else
            {
                this->require_start = parts.front();
                this->require_seq = std::vector<std::string>(parts.begin() + 1, parts.end() - 1);
                this->require_end = parts.back();
            }
        }

        like::like (const std::string& query) : like(split(query, '%')) {}

        bool like::operator()(const field& field)
        {
            if (field.type != value_type::TEXT && field.type != value_type::VARCHAR)
            {
                return false;
            }
            if (this->require_empty)
            {
                return field.str_val.empty();
            }
            if (!this->require_exact.empty())
            {
                return field.str_val == this->require_exact;
            }
            if (!this->require_start.empty() && !starts_with(field.str_val, this->require_start))
            {
                return false;
            }
            if (!this->require_end.empty() && !ends_with(field.str_val, this->require_end))
            {
                return false;
            }

            std::string::const_iterator it = field.str_val.begin();
            for (const std::string& part : this->require_seq)
            {
                it = std::search(it, field.str_val.end(), part.begin(), part.end());
                if (it == field.str_val.end())
                {
                    return false;
                }
                it += part.length();
            }

            return true;
        }

        int_eq::int_eq(int require_exact) : require_exact(require_exact)
        {}

        bool int_eq::operator()(const field& field)
        {
            if (field.type != value_type::INTEGER) return false;
            return field.int_val == this->require_exact;
        }
    }
}
