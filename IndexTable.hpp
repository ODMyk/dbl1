#ifndef INDEX_TABLE_HPP
#define INDEX_TABLE_HPP

#include <queue>
#include <map>
#include <fstream>

struct Index
{
    __int32 id = 0;
    __int32 address = -1;
};

struct IndexTable
{
    std::queue<__int32> free_ids;
    std::map<__int32, __int32> records;
    __int32 counter = 0;
};

bool load_index_table(IndexTable &dst, std::fstream &src);

bool export_index_table(std::fstream &dst, IndexTable &src);

#endif