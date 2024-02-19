#ifndef GARBAGE_HPP
#define GARBAGE_HPP

#include <fstream>
#include <vector>

bool load_trash(std::vector<__int32> &trash, std::fstream &file);
bool export_trash(std::fstream &file, std::vector<__int32> &trash);

#endif