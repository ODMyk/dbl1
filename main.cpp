#include <iostream>
#include <map>
#include <string.h>

#include "io_utils.hpp"
#include "IndexTable.hpp"
#include "Commands.hpp"
#include "Garbage.hpp"

void mainloop(std::vector<__int32> &trash, IndexTable &artists_index_table, std::fstream &artists_records_file, IndexTable &songs_index_table, std::fstream &songs_records_file, std::string &slave_filename);

void replaceSubStr(
    std::string &str, const std::string &subStr, const std::string &replacement)
{

    size_t pos = str.find(subStr);
    while (pos != std::string::npos)
    {
        str.replace(pos, subStr.length(), replacement);
        pos = str.find(subStr, pos + replacement.length());
    }
}

void parse_line(std::string line, std::string &command, std::vector<std::string> &args)
{
    int ind, ind2;
    std::string arg;

    ind = line.find_first_not_of(' ', 0);
    if (ind == std::string::npos)
    {
        command = "";
        return;
    }
    command = line.substr(ind, line.find_first_of(' ', ind) - ind);
    {
        int tind = line.find_first_of(' ', ind);
        if (tind == std::string::npos)
        {
            return;
        }
        tind = line.find_first_not_of(' ', ind + command.size());
        if (tind == std::string::npos)
        {
            return;
        }
        ind2 = tind;
    }
    line = line.substr(ind2, line.size() - 1 - command.size() - ind);

    while (ind != std::string::npos)
    {
        ind = line.find_first_of(' ', 0);
        arg = line.substr(0, ind);
        replaceSubStr(arg, "_", " ");
        args.push_back(arg);
        int ind2 = line.find_first_not_of(' ', ind + 1);
        if (ind2 != std::string::npos)
        {
            line = line.substr(ind2, line.size() - 1 - arg.size());
        }
        else
        {
            ind = ind2;
        }
    }
}

void mainloop(std::vector<__int32> &trash, IndexTable &artists_index_table, std::fstream &artists_records_file, IndexTable &songs_index_table, std::fstream &songs_records_file, std::string &slave_filename)
{
    std::map<std::string, Command> commands = {};
    configure_commands(commands);

    std::string command;
    std::string line;
    std::vector<std::string> args;
    while (true)
    {
        std::cout << " $ ";
        std::getline(std::cin, line);
        args.clear();
        parse_line(line, command, args);

        if (!strcmp(command.c_str(), "exit"))
        {
            return;
        }

        if (!commands.contains(command))
        {
            command = "error";
        }

        commands.at(command)(trash, artists_index_table, artists_records_file, songs_index_table, songs_records_file, args, slave_filename);
    }
}

int main(int argc, char **argv)
{

    std::string artists_path = "artists";
    std::string songs_path = "songs";
    std::string trash_path = "trash";
    if (argc == 2 || argc > 4)
    {
        std::cerr << "Invalid input data. Please, use 2, 3 or 0 filenames" << std::endl;
        return -1;
    }

    if (argc > 2)
    {
        artists_path = argv[1];
        songs_path = argv[2];
    }
    if (argc == 4)
    {
        trash_path = argv[3];
    }

    std::fstream artists_index_file, artists_records_file, songs_index_file, songs_records_file, trash_file;
    if (!open_files(artists_index_file, artists_records_file, artists_path) || !open_files(songs_index_file, songs_records_file, songs_path) || !open_trash(trash_file, trash_path))
    {
        std::cerr << "There was a problem with opening files. If you passes your filenames, please make sure that all files are exist and have appropriate extensions" << std::endl;
        return -1;
    }

    IndexTable artists_index_table, songs_index_table;
    std::vector<__int32> trash;

    if (!load_index_table(artists_index_table, artists_index_file) || !load_index_table(songs_index_table, songs_index_file))
    {
        std::cerr << "Unable to read index tables, shuting down" << std::endl;
        return -1;
    }
    artists_index_file.close();
    songs_index_file.close();
    if (!load_trash(trash, trash_file))
    {
        std::cerr << "Unable to read garbage-file, shuting down" << std::endl;
        return -1;
    }
    trash_file.close();

    std::string tmp = songs_path + ".bin";

    mainloop(trash, artists_index_table, artists_records_file, songs_index_table, songs_records_file, tmp);

    trash_file.open(trash_path + ".gb", std::ios::binary | std::ios::out);
    export_trash(trash_file, trash);
    trash_file.close();

    artists_index_file.open(artists_path + ".ind", std::ios::binary | std::ios::out);
    export_index_table(artists_index_file, artists_index_table);
    artists_index_file.close();
    songs_index_file.open(songs_path + ".ind", std::ios::binary | std::ios::out);
    export_index_table(songs_index_file, songs_index_table);
    songs_index_file.close();

    artists_records_file.close();
    songs_records_file.close();

    return 0;
}