#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <fstream>
#include <map>
#include <string.h>
#include <iostream>

#include "IndexTable.hpp"
#include "Artist.hpp"
#include "Song.hpp"
#include "io_utils.hpp"

typedef void (*Command) (std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);

void parse_line(std::string line, std::string& command, std::vector<std::string> &args);
void configure_commands(std::map<std::string, Command> &commands);

void print_help(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void print_error(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void read_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void read_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void insert_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void insert_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void delete_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void delete_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void update_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void update_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void calculate_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void calculate_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void readall_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);
void readall_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string>& args, std::string &slave_filename);


#endif