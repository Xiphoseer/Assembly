#include "scene.hpp"

namespace assembly::scene
{
    chunk::chunk(chunk_type t)
    {
        this->type = t;
        switch (this->type)
        {
            case chunk_type::chnk_1000: new(&this->v1000) chunk_1000; break;
            case chunk_type::chnk_2000: new(&this->v2000) chunk_2000; break;
            case chunk_type::chnk_2001: new(&this->v2001) chunk_2001; break;
            case chunk_type::chnk_2002: new(&this->v2002) chunk_2002; break;
        }
    };

    chunk::chunk(const chunk& c)
    {
        this->type = c.type;
        switch (this->type)
        {
            case chunk_type::chnk_1000: new(&this->v1000) chunk_1000; this->v1000 = c.v1000; break;
            case chunk_type::chnk_2000: new(&this->v2000) chunk_2000; this->v2000 = c.v2000; break;
            case chunk_type::chnk_2001: new(&this->v2001) chunk_2001; this->v2001 = c.v2001; break;
            case chunk_type::chnk_2002: new(&this->v2002) chunk_2002; this->v2002 = c.v2002; break;
        }
    };

    chunk::~chunk()
    {
        switch (this->type)
        {
            case chunk_type::chnk_1000: this->v1000.~chunk_1000(); break;
            case chunk_type::chnk_2000: this->v2000.~chunk_2000(); break;
            case chunk_type::chnk_2001: this->v2001.~chunk_2001(); break;
            case chunk_type::chnk_2002: this->v2002.~chunk_2002(); break;
        }
    }
}
