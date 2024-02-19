#include "Garbage.hpp"
#include "io_utils.hpp"

bool load_trash(std::vector<__int32> &trash, std::fstream &file)
{
    __int32 count;
    __int32 pos = 0;
    __int32 tmp;
    if (!read(count, file, pos))
    {
        return false;
    }
    while (count-- > 0)
    {
        pos += sizeof(__int32);
        if (!read(tmp, file, pos))
        {
            return false;
        }
        trash.push_back(tmp);
    }

    return true;
}

bool export_trash(std::fstream &file, std::vector<__int32> &trash)
{
    __int32 pos = trash.size();

    if (!write(file, pos, 0))
    {
        return false;
    }
    pos = 0;
    for (__int32 i = 0; i < trash.size(); ++i)
    {
        pos += sizeof(__int32);
        if (!write(file, trash.at(i), pos))
        {
            return false;
        }
    }

    return true;
}
