#include "IndexTable.hpp"
#include "io_utils.hpp"

bool load_index_table(IndexTable &dst, std::fstream &src)
{
    if (!src)
    {
        return false;
    }

    unsigned i = 0;
    __int32 free_count = 0;
    __int32 records_count = 0;

    if (!read(free_count, src, 0))
    {
        return false;
    }
    i += sizeof(__int32);

    if (!read(records_count, src, i))
    {
        return false;
    }
    i += sizeof(__int32);

    if (!read(dst.counter, src, i))
    {
        return false;
    }
    i += sizeof(__int32);

    while (dst.free_ids.size() < free_count)
    {
        __int32 tmp;
        if (!read(tmp, src, i))
        {
            return false;
        }
        dst.free_ids.push(tmp);
        i += sizeof(__int32);
    }

    while (dst.records.size() < records_count)
    {
        Index tmp;
        if (!read(tmp, src, i))
        {
            return false;
        }
        dst.records[tmp.id] = tmp.address;
        i += sizeof(Index);
    }

    return !src.fail();
}

bool export_index_table(std::fstream &dst, IndexTable &src)
{
    if (!dst)
    {
        return false;
    }

    unsigned i = 0;
    __int32 free_count = (__int32)src.free_ids.size();
    __int32 records_count = (__int32)src.records.size();

    if (!write(dst, free_count, i))
    {
        return false;
    }
    i += sizeof(__int32);

    if (!write(dst, records_count, i))
    {
        return false;
    }
    i += sizeof(__int32);

    if (!write(dst, src.counter, i))
    {
        return false;
    }
    i += sizeof(__int32);

    for (int j = 0; j < free_count; ++j)
    {
        if (!write(dst, src.free_ids.front(), i))
        {
            return false;
        }
        src.free_ids.pop();
        i += sizeof(__int32);
    }

    for (auto j = src.records.begin(); j != src.records.end(); ++j)
    {
        Index tmp = {j->first, j->second};
        if (!write(dst, tmp, i))
        {
            return false;
        }
        i += sizeof(Index);
    }

    return !dst.fail();
}