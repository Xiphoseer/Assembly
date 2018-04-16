#pragma once
#include "scene.hpp"

/*! \file scene_io.hpp
 *  \brief Reading scene files
 *
 *  Reading utilities for scenes
 */

namespace assembly::scene::io
{
    //! read scene from stream
    int read_from_stream(std::istream& i_strm, scene_file& lvl);
    //! read scene from file
    int read_from_file(const std::string& s_file, scene_file& lvl);
}
