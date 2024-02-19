#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <fstream>
#include "Artist.hpp"
#include "Song.hpp"
#include <string>
#include "IndexTable.hpp"

bool read(Artist &dst, std::fstream &src, const std::streampos &pos);
bool read(Song &dst, std::fstream &src, const std::streampos &pos);
bool read(Index &dst, std::fstream &src, const std::streampos &pos);
bool read(int &dst, std::fstream &src, const std::streampos &pos);

bool write(std::fstream &dst, Artist &src, const std::streampos &pos);
bool write(std::fstream &dst, Song &src, const std::streampos &pos);
bool write(std::fstream &dst, Index &src, const std::streampos &pos);
bool write(std::fstream &dst, int &src, const std::streampos &pos);

bool open_files(std::fstream &index_file, std::fstream &records_file, std::string filepath);
bool open_trash(std::fstream &file, std::string filepath);

bool reorganize(std::fstream &slave_file, std::fstream &master_file, IndexTable &master_table, IndexTable &table, std::vector<__int32> &trash, std::string &filename);

#endif