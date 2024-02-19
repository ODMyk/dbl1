#include "io_utils.hpp"
#include <algorithm>
#include <filesystem>

bool read(Artist &dst, std::fstream &src, const std::streampos &pos)
{

    if (!src)
    {
        return false;
    }

    src.seekg(pos);
    src.read(reinterpret_cast<char *>(&dst), sizeof(Artist));

    return !src.fail();
}

bool read(Song &dst, std::fstream &src, const std::streampos &pos)
{

    if (!src)
    {
        return false;
    }

    src.seekg(pos);
    src.read(reinterpret_cast<char *>(&dst), sizeof(Song));

    return !src.fail();
}

bool read(Index &dst, std::fstream &src, const std::streampos &pos)
{

    if (!src)
    {
        return false;
    }

    src.seekg(pos);
    src.read(reinterpret_cast<char *>(&dst), sizeof(Index));

    return !src.fail();
}

bool read(int &dst, std::fstream &src, const std::streampos &pos)
{

    if (!src)
    {
        return false;
    }

    src.seekg(pos);
    src.read(reinterpret_cast<char *>(&dst), sizeof(int));

    return !src.fail();
}

bool write(std::fstream &dst, Artist &src, const std::streampos &pos)
{
    if (!dst)
    {
        return false;
    }

    dst.seekp(pos);
    dst.write(reinterpret_cast<char *>(&src), sizeof(Artist));
    dst.flush();

    return !dst.fail();
}

bool write(std::fstream &dst, Song &src, const std::streampos &pos)
{
    if (!dst)
    {
        return false;
    }

    dst.seekp(pos);
    dst.write(reinterpret_cast<char *>(&src), sizeof(Song));
    dst.flush();

    return !dst.fail();
}

bool write(std::fstream &dst, Index &src, const std::streampos &pos)
{
    if (!dst)
    {
        return false;
    }

    dst.seekp(pos);
    dst.write(reinterpret_cast<char *>(&src), sizeof(Index));
    dst.flush();

    return !dst.fail();
}

bool write(std::fstream &dst, int &src, const std::streampos &pos)
{
    if (!dst)
    {
        return false;
    }

    dst.seekp(pos);
    dst.write(reinterpret_cast<char *>(&src), sizeof(int));
    dst.flush();

    return !dst.fail();
}

bool open_files(std::fstream &index_file, std::fstream &records_file, std::string filepath)
{
    auto mode = std::ios::binary | std::ios::in | std::ios::out;
    index_file.close();
    records_file.close();

    index_file.open(filepath + ".ind", mode);
    records_file.open(filepath + ".bin", mode);

    if (index_file.is_open() ^ records_file.is_open())
    {
        index_file.close();
        records_file.close();
        return false;
    }

    if (!index_file.is_open())
    {
        mode = mode | std::ios::trunc;
        __int32 zero = 0;

        index_file.open(filepath + ".ind", mode);
        if (index_file.is_open())
        {
            for (int i = 0; i < 3; ++i)
            {
                write(index_file, zero, i * sizeof(__int32));
            }
        }
        records_file.open(filepath + ".bin", mode);
    }

    return index_file.is_open() && records_file.is_open();
}

bool open_trash(std::fstream &file, std::string filepath)
{
    auto mode = std::ios::binary | std::ios::out | std::ios::in;
    file.close();
    file.open(filepath + ".gb", mode);
    if (!file.is_open())
    {
        mode |= std::ios::trunc;
        file.open(filepath + ".gb", mode);
        if (!file.is_open())
        {
            return false;
        }
        int zero = 0;
        write(file, zero, zero);
    }
    return file.is_open();
}

bool cmp(Index &l, Index &r)
{
    return l.address >= r.address;
}

bool reorganize(std::fstream &slave_file, std::fstream &master_file, IndexTable &master_table, IndexTable &table, std::vector<__int32> &trash, std::string &filename)
{
    std::vector<Index> records_order;
    for (auto i = table.records.begin(); i != table.records.end(); ++i)
    {
        records_order.push_back({i->first, i->second});
    }
    std::sort(records_order.begin(), records_order.end(), cmp);
    __int32 i = 0;
    while (i < records_order.size() && i < TRASH_CAPACITY && records_order[i].address > trash[i])
    {
        __int32 cur_addr = records_order.at(i).address;
        __int32 addr = trash.at(i);
        Song tmp;
        Artist master;
        read(tmp, slave_file, cur_addr);
        read(master, master_file, master_table.records.at(tmp.artist_id));
        if (master.head == cur_addr)
        {
            master.head = addr;
        }
        if (master.tail == cur_addr)
        {
            master.tail = addr;
        }
        if (master.head == addr || master.tail == addr)
        {
            write(master_file, master, master_table.records.at(tmp.artist_id));
        }
        if (tmp.prev != ABSENT)
        {
            Song p;
            read(p, slave_file, tmp.prev);
            p.next = addr;
            write(slave_file, p, tmp.prev);
        }
        if (tmp.next != ABSENT)
        {
            Song n;
            read(n, slave_file, tmp.next);
            n.prev = addr;
            write(slave_file, n, tmp.next);
        }
        write(slave_file, tmp, addr);
        table.records[tmp.id] = addr;
        ++i;
    }
    trash.clear();
    slave_file.close();
    std::filesystem::resize_file(filename, table.records.size() * sizeof(Song));
    slave_file.open(filename, std::ios::binary | std::ios::in | std::ios::out);

    return slave_file.is_open();
}
