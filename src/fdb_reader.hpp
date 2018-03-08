#pragma once

#include "filedatabase.hpp"
#include <iostream>


namespace FDB
{
    int readFromStream(std::istream& strm, FDB::Schema& schema);
    int readFromFile(const std::string& filename, FDB::Schema& schema);
}
