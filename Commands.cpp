#include "Commands.hpp"
#include "Constants.hpp"
#include <algorithm>

void print_invalid_parameters()
{
    std::cout << "Invalid parameters set. Please, use command 'help' to read more about parameters" << std::endl;
}

void print_invalid_id()
{
    std::cout << "Invalid identifier passed" << std::endl;
}

void configure_commands(std::map<std::string, Command> &commands)
{
    commands["error"] = print_error;
    commands["help"] = print_help;
    commands["get-m"] = read_master;
    commands["get-s"] = read_slave;
    commands["insert-m"] = insert_master;
    commands["insert-s"] = insert_slave;
    commands["del-m"] = delete_master;
    commands["del-s"] = delete_slave;
    commands["update-m"] = update_master;
    commands["update-s"] = update_slave;
    commands["ut-m"] = readall_master;
    commands["ut-s"] = readall_slave;
    commands["calc-m"] = calculate_master;
    commands["calc-s"] = calculate_slave;
}

void parse_id(int &id, std::string parameter)
{
    if (!strcmp("all", parameter.c_str()))
    {
        id = 0;
        return;
    }
    try
    {
        id = std::stoi(parameter);
    }
    catch (std::exception)
    {
        id = -1;
    }
}

bool parse_fields(std::vector<std::string> args, const std::string *field_names, bool *fields_to_print, const unsigned size)
{
    for (int i = 1; i < args.size(); ++i)
    {
        int j;
        for (j = 0; j < size; ++j)
        {
            if (!strcmp(args[i].c_str(), field_names[j].c_str()))
            {
                fields_to_print[j] = true;
                break;
            }
        }
        if (j == size)
        {
            return false;
        }
    }
    return true;
}

void print_help(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    std::cout << "\nAvailable commands:\n\texit - stop the program\n\n\thelp - display this message\n\n\tget-m {id/'all'} [fieldname, ...] - display master records\n\n\tget-s {id/'all'/'from='master_id} [fieldname, ...] - display slave records\n\n\tinsert-m nickname - create new master record\n\n\tinsert-s title times_played artist_id - create new slave record\n\n\tdel-m id - delete master record\n\n\tdel-s id - delete slave record\n\n\tcalc-m expanded=false - display master records count and (if expanded=true) count of subrecords for each record\n\n\tcalc-s - display slave records count\n\n\tut-m - display all master records with all fields (including hidden)\n\n\tut-s - display all slave records with all fields(including hidden)\n\n\tupdate-m id [fielname=value, ...] - update master record\n\n\tupdate-s id [fieldname=value, ...] - update slave record" << std::endl;
}

void print_error(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    std::cout << "Unrecognized command. Please, use command 'help' to see available commands" << std::endl;
}

void print_not_read()
{
    std::cout << "There was a problem with reading file" << std::endl;
}

void print_record(Artist &record, const bool fields[5])
{
    if (fields[0])
    {
        std::cout << record.id << " ";
    }
    if (fields[1])
    {
        std::cout << record.nickname << " ";
    }
    if (fields[2])
    {
        std::cout << record.head << " ";
    }
    if (fields[3])
    {
        std::cout << record.tail << " ";
    }
    if (fields[4])
    {
        std::cout << record.subrecords_count;
    }
    std::cout << std::endl;
}

void print_record(Song &record, const bool fields[6])
{
    if (fields[0])
    {
        std::cout << record.id << " ";
    }
    if (fields[1])
    {
        std::cout << record.title << " ";
    }
    if (fields[2])
    {
        std::cout << record.times_played << " ";
    }
    if (fields[3])
    {
        std::cout << record.artist_id << " ";
    }
    if (fields[5])
    {
        std::cout << record.prev << " ";
    }
    if (fields[4])
    {
        std::cout << record.next;
    }
    std::cout << std::endl;
}

void print_all_master(IndexTable &table, std::fstream &file, const bool fields[3])
{
    for (auto i = table.records.begin(); i != table.records.end(); ++i)
    {
        Artist record;
        if (!read(record, file, i->second))
        {
            throw std::exception();
        }
        print_record(record, fields);
    }
}

void print_all_slave(IndexTable &table, std::fstream &file, const bool fields[6])
{
    for (auto i = table.records.begin(); i != table.records.end(); ++i)
    {
        Song record;
        if (!read(record, file, i->second))
        {
            throw std::exception();
        }
        print_record(record, fields);
    }
}

void read_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    if (args.size() < 1 || args.size() > READABLE_FIELDS_MASTER + 1)
    {
        print_invalid_parameters();
        return;
    }
    int id;
    parse_id(id, args[0]);

    if (id < 0)
    {
        print_invalid_id();
        return;
    }
    const std::string field_names[] = {"id", "nickname"};
    bool fields[] = {false, false, false, false, false};
    if (args.size() == 1)
    {
        fields[0] = (fields[1] = true);
    }
    else
    {
        if (!parse_fields(args, field_names, fields, READABLE_FIELDS_MASTER))
        {
            std::cout << "Unrecognized fieldname was passed" << std::endl;
            return;
        }
    }

    if (id == 0)
    {
        print_all_master(master_index_table, master_records_file, fields);
        return;
    }

    Artist record;
    if (!master_index_table.records.contains(id))
    {
        std::cout << "Record was not found" << std::endl;
        return;
    }

    if (!read(record, master_records_file, master_index_table.records[id]))
    {
        print_not_read();
        return;
    }

    print_record(record, fields);
}

void print_subrecords(Artist &a, const bool fields[5], IndexTable &table, std::fstream &file)
{
    std::cout << "Subrecords of Artist with ID:" << a.id << "\n{\n";
    int i = a.head;
    Song s;
    while (i != ABSENT)
    {
        read(s, file, i);
        print_record(s, fields);
        i = s.next;
    }
    std::cout << "}\nTotal: " << a.subrecords_count << std::endl;
}

void read_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    if (args.size() < 1 || args.size() > READABLE_FIELDS_SLAVE + 1)
    {
        print_invalid_parameters();
        return;
    }
    int id;
    bool subprint = false;
    parse_id(id, args[0]);

    if (id < 0)
    {
        if (args[0].starts_with("from="))
        {
            subprint = true;
            try
            {
                id = std::stoi(args[0].substr(args[0].find_first_of("=") + 1));
                if (!master_index_table.records.contains(id))
                {
                    throw std::exception();
                }
            }
            catch (std::exception)
            {
                print_invalid_id();
                return;
            }
        }
        else
        {
            print_invalid_id();
            return;
        }
    }
    const std::string field_names[] = {"id", "title", "times_played", "artist_id"};
    bool fields[] = {false, false, false, false, false, false};
    if (args.size() == 1)
    {
        fields[0] = fields[1] = fields[2] = true;
    }
    else
    {
        if (!parse_fields(args, field_names, fields, READABLE_FIELDS_SLAVE))
        {
            std::cout << "Unrecognized fieldname was passed" << std::endl;
            return;
        }
    }

    if (subprint)
    {
        Artist a;
        read(a, master_records_file, master_index_table.records[id]);
        print_subrecords(a, fields, slave_index_table, slave_records_file);
        return;
    }

    if (id == 0)
    {
        print_all_slave(slave_index_table, slave_records_file, fields);
        return;
    }

    Song record;
    if (!slave_index_table.records.contains(id))
    {
        std::cout << "Record was not found" << std::endl;
        return;
    }

    if (!read(record, slave_records_file, slave_index_table.records[id]))
    {
        print_not_read();
        return;
    }

    print_record(record, fields);
}

void insert_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    if (args.size() != REQUIRED_FIELDS_MASTER || args[0].size() > STRING_LENGTH)
    {
        print_invalid_parameters();
        return;
    }

    int id;
    if (master_index_table.free_ids.empty())
    {
        id = ++master_index_table.counter;
    }
    else
    {
        id = master_index_table.free_ids.front();
        master_index_table.free_ids.pop();
    }

    Artist a = {id, {}, ABSENT, ABSENT, 0};
    int addr = master_index_table.records.size() * sizeof(Artist);

    strcpy(a.nickname, args[0].c_str());

    write(master_records_file, a, addr);

    master_index_table.records[id] = addr;
}

void insert_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    if (args.size() != REQUIRED_FIELDS_SLAVE || args[0].size() > STRING_LENGTH)
    {
        print_invalid_parameters();
        return;
    }

    int id;
    if (slave_index_table.free_ids.empty())
    {
        id = ++slave_index_table.counter;
    }
    else
    {
        id = slave_index_table.free_ids.front();
        slave_index_table.free_ids.pop();
    }

    Song s = {id, {}, 0, ABSENT, ABSENT, ABSENT};
    int addr;
    if (trash.size() == 0)
    {
        addr = slave_index_table.records.size() * sizeof(Song);
    }
    else
    {
        addr = trash.at(0);
    }

    strcpy(s.title, args[0].c_str());

    try
    {
        s.times_played = std::stoi(args[1]);
    }
    catch (std::exception)
    {
        print_invalid_parameters();
    }
    try
    {
        s.artist_id = std::stoi(args[2]);
    }
    catch (std::exception)
    {
        print_invalid_parameters();
    }

    Artist a;
    if (!master_index_table.records.contains(s.artist_id))
    {
        print_invalid_id();
        return;
    }
    read(a, master_records_file, master_index_table.records[s.artist_id]);

    if (a.head == ABSENT)
    {
        a.head = a.tail = addr;
    }
    else
    {
        Song t;
        read(t, slave_records_file, a.tail);
        t.next = addr;
        s.prev = a.tail;
        write(slave_records_file, t, a.tail);
        a.tail = addr;
    }

    ++a.subrecords_count;
    if (trash.size() != 0)
    {
        trash.erase(trash.begin());
    }

    write(slave_records_file, s, addr);
    write(master_records_file, a, master_index_table.records[s.artist_id]);

    slave_index_table.records[id] = addr;
}

void delete_slave(__int32 id, __int32 pos, IndexTable &table, std::vector<__int32> &trash)
{
    table.free_ids.push(id);
    trash.push_back(pos);
    table.records.erase(id);
}

int delete_slave(IndexTable &table, std::fstream &file, int pos, std::vector<__int32> &trash)
{
    Song cur;
    read(cur, file, pos);
    delete_slave(cur.id, pos, table, trash);

    return cur.next;
}

void modify_list(Song &cur, std::fstream &file)
{
    Song prev, next;
    read(prev, file, cur.prev);
    read(next, file, cur.next);
    next.prev = cur.prev;
    prev.next = cur.next;
    write(file, next, cur.next);
    write(file, prev, cur.prev);
}

void handle_trash(std::fstream &slave_file, std::fstream &master_file, IndexTable &master_table, IndexTable &table, std::vector<__int32> &trash, std::string &filename)
{
    std::sort(trash.begin(), trash.end());
    if (trash.size() >= TRASH_CAPACITY)
    {
        reorganize(slave_file, master_file, master_table, table, trash, filename);
    }
}

void delete_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    if (args.size() != 1)
    {
        print_invalid_parameters();
        return;
    }
    int id;
    parse_id(id, args[0]);
    if (id < 1 || !master_index_table.records.contains(id))
    {
        print_invalid_id();
        return;
    }
    Artist a_to_delete;
    int cur_addr = master_index_table.records[id];
    read(a_to_delete, master_records_file, cur_addr);
    int addr = (master_index_table.records.size() - 1) * sizeof(Artist);
    if (addr != cur_addr)
    {
        Artist a;
        read(a, master_records_file, addr);
        write(master_records_file, a, cur_addr);
        master_index_table.records[a.id] = cur_addr;
    }

    master_index_table.free_ids.push(id);
    master_index_table.records.erase(id);

    addr = a_to_delete.head;
    while (addr != -1)
    {
        addr = delete_slave(slave_index_table, slave_records_file, addr, trash);
    }
    handle_trash(slave_records_file, master_records_file, master_index_table, slave_index_table, trash, slave_filename);
}

void delete_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    if (args.size() != 1)
    {
        print_invalid_parameters();
        return;
    }
    int id;
    parse_id(id, args[0]);
    if (id < 1 || !slave_index_table.records.contains(id))
    {
        print_invalid_id();
        return;
    }
    Song cur;
    Song tmp;
    __int32 addr = slave_index_table.records[id];
    Artist master;
    read(cur, slave_records_file, addr);
    __int32 mpos = master_index_table.records[cur.artist_id];
    read(master, master_records_file, mpos);
    --master.subrecords_count;
    bool toMod = true;
    if (master.head == addr)
    {
        master.head = cur.next;
        if (master.head != ABSENT)
        {
            read(tmp, slave_records_file, master.head);
            tmp.prev = ABSENT;
            write(slave_records_file, tmp, master.head);
        }
        toMod = false;
    }
    if (master.tail == addr)
    {
        master.tail = cur.prev;
        if (master.tail != ABSENT)
        {
            read(tmp, slave_records_file, master.tail);
            tmp.next = ABSENT;
            write(slave_records_file, tmp, master.tail);
        }
        toMod = false;
    }
    write(master_records_file, master, mpos);
    delete_slave(cur.id, addr, slave_index_table, trash);
    if (toMod)
    {
        modify_list(cur, slave_records_file);
    }
    handle_trash(slave_records_file, master_records_file, master_index_table, slave_index_table, trash, slave_filename);
}

bool parse_update_keys_and_values(std::vector<std::string> &args, std::map<std::string, std::string> &dict)
{
    for (int i = 1; i < args.size(); ++i)
    {
        std::string line = args.at(i);
        int ind = line.find_first_of('=');
        if (ind == std::string::npos)
        {
            return false;
        }
        std::string key;
        key = line.substr(0, ind);
        line = line.substr(ind + 1, line.size() - 1 - key.size());
        dict[key] = line;
    }
    return true;
}

void update_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    if (args.size() < 2 || args.size() > MODIFIABLE_FIELDS_MASTER + 1)
    {
        print_invalid_parameters();
        return;
    }
    std::map<std::string, std::string> fields;
    if (!parse_update_keys_and_values(args, fields))
    {
        print_invalid_parameters();
        return;
    }
    std::string nickname;

    for (auto i = fields.begin(); i != fields.end(); ++i)
    {
        if (!i->first.compare("nickname"))
        {
            nickname = i->second;
            continue;
        }
        print_invalid_parameters();
        return;
    }

    int id;
    parse_id(id, args[0]);
    if (id < 1 || !master_index_table.records.contains(id))
    {
        print_invalid_id();
        return;
    }
    id = master_index_table.records[id];
    Artist a;
    read(a, master_records_file, id);
    strcpy(a.nickname, nickname.c_str());
    write(master_records_file, a, id);
}

void update_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    if (args.size() < 2 || args.size() > MODIFIABLE_FIELDS_SLAVE + 1)
    {
        print_invalid_parameters();
        return;
    }
    std::map<std::string, std::string> fields;
    if (!parse_update_keys_and_values(args, fields))
    {
        print_invalid_parameters();
        return;
    }
    std::string title;
    int times_played = ABSENT;

    for (auto i = fields.begin(); i != fields.end(); ++i)
    {
        if (!i->first.compare("title"))
        {
            title = i->second;
            continue;
        }
        if (!i->first.compare("times played"))
        {
            try
            {
                times_played = std::stoi(i->second);
                if (times_played >= 0)
                {
                    continue;
                }
            }
            catch (std::exception)
            {
            } // loop isn't broken, so necessary action will be performed
        }
        print_invalid_parameters();
        return;
    }

    int id;
    parse_id(id, args[0]);
    if (id < 1 || !slave_index_table.records.contains(id))
    {
        print_invalid_id();
        return;
    }
    id = slave_index_table.records[id];
    Song s;
    bool should_write = false;
    read(s, slave_records_file, id);

    if (title.compare("") && title.compare(s.title))
    {
        strcpy(s.title, title.c_str());
        should_write = true;
    }
    if (times_played >= 0 && times_played != s.times_played)
    {
        s.times_played = times_played;
        should_write = true;
    }
    if (should_write)
    {
        write(slave_records_file, s, id);
    }
}

void calculate_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    if (args.size() > 1)
    {
        print_invalid_parameters();
        return;
    }
    bool t = !strcmp(args[0].c_str(), "true");
    bool f = !strcmp(args[0].c_str(), "false");
    if (!t && !f)
    {
        std::cout << "Invalid option was passed. The available ones are 'true' and 'false'" << std::endl;
        return;
    }
    if (f)
    {
        std::cout << "Count of records in artists table: " << master_index_table.records.size() << std::endl;
        return;
    }

    for (auto i = master_index_table.records.begin(); i != master_index_table.records.end(); ++i)
    {
        Artist record;
        if (!read(record, master_records_file, i->second))
        {
            std::cout << "Unable to read record" << std::endl;
            return;
        }
        std::cout << "Record with ID:" << record.id << " has " << record.subrecords_count << " subrecords" << std::endl;
    }

    std::cout << "Total artists count: " << master_index_table.records.size() << std::endl;
}

void calculate_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    std::cout << "Count of records is songs table: " << slave_index_table.records.size() << std::endl;
}

void readall_master(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    const bool fields[] = {true, true, true, true, true};
    print_all_master(master_index_table, master_records_file, fields);
}

void readall_slave(std::vector<__int32> &trash, IndexTable &master_index_table, std::fstream &master_records_file, IndexTable &slave_index_table, std::fstream &slave_records_file, std::vector<std::string> &args, std::string &slave_filename)
{
    const bool fields[] = {true, true, true, true, true, true};
    print_all_slave(slave_index_table, slave_records_file, fields);
}
