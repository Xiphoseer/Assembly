#include "fdb_query.hpp"

#include <algorithm>

#include "filedatabase.hpp"
#include "stringutil.hpp"

namespace FDB
{
    namespace Query
    {
        Like::Like (const std::vector<std::string>& parts)
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

        Like::Like (const std::string& query) : Like(split(query, '%')) {}

        bool Like::operator()(const Field& field)
        {
            if (field.type != ValType::TEXT && field.type != ValType::VARCHAR)
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
    }
}
