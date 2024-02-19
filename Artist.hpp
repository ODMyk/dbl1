#ifndef ARTIST_HPP
#define ARTIST_HPP

#include <iostream>
#include "Constants.hpp"

struct Artist
{
    __int32 id = 0;
    char nickname[STRING_LENGTH] = {};
    __int32 head = ABSENT;
    __int32 tail = ABSENT;
    __int32 subrecords_count = 0;
};

#endif