#ifndef SONG_HPP
#define SONG_HPP

#include <iostream>
#include "Constants.hpp"

struct Song
{
    __int32 id = 0;
    __int32 artist_id = 0;
    char title[STRING_LENGTH] = {};
    __int32 times_played = 0;
    __int32 next = ABSENT;
    __int32 prev = ABSENT;
};

#endif