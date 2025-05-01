// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
//  simulation.h
//  p3-sillyql
//
//  Created by Joyce Zhou on 3/21/23.
//

#ifndef simulation_h
#define simulation_h

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <getopt.h>
#include "TableEntry.h"
using namespace std;

enum Index : uint32_t {
    none, hash, bst
};

class greaterThan {
public:
    greaterThan(uint32_t column_in, TableEntry compare_in): column_index(column_in), compare(compare_in) {}
    bool operator() (const vector<TableEntry> &row) {
        return row[column_index] > compare;
    }
private:
    uint32_t column_index;
    TableEntry compare;
};

class lessThan {
public:
    lessThan(uint32_t column_in, TableEntry compare_in): column_index(column_in), compare(compare_in) {}
    bool operator() (const vector<TableEntry> &row) {
        return row[column_index] < compare;
    }
private:
    uint32_t column_index;
    TableEntry compare;
};

class equalTo {
public:
    equalTo(uint32_t column_in, TableEntry compare_in): column_index(column_in), compare(compare_in) {}
    bool operator() (const vector<TableEntry> &row) {
        return row[column_index] == compare;
    }
private:
    uint32_t column_index;
    TableEntry compare;
};

struct Table {
    vector<string> col_names;
    vector<EntryType> col_types;
    vector<vector<TableEntry>> entries;
    unordered_map<TableEntry, vector<uint32_t>> hash;
    map<TableEntry, vector<uint32_t>, less<TableEntry>> bst;
    Index index_type = Index::none;
    uint32_t num_cols = 0;
    string col_hash = "";
};

class Simulation {
public:
    void getOutputMode(int argc, char * argv[]);
    void processInput();
    void createTable(string table_name);
    void insertEntry(string table_name, uint32_t add_rows);
    void printEntries(string table_name);
    void deleteEntries(string table_name);
    void generateIndex(string table_name);
    void joinTables(string table_name1, string table_name2);
    
private:
    unordered_map<string, Table> database;
    bool quietMode = false;
    
    TableEntry deleteHelper(string table_name, uint32_t index) {
        Table* table = &database[table_name];
        switch(table->col_types[index]) {
            case EntryType::String: {
                string entry_string;
                cin >> entry_string;
                return TableEntry(entry_string);
                break;
            }
            case EntryType::Double: {
                double entry_double;
                cin >> entry_double;
                return TableEntry(entry_double);
                break;
            }
            case EntryType::Int: {
                int entry_int;
                cin >> entry_int;
                return TableEntry(entry_int);
                break;
            }
            case EntryType::Bool: {
                bool entry_bool;
                cin >> entry_bool;
                return TableEntry(entry_bool);
                break;
            }
        }
        return TableEntry(0);
    }
    
    void regenerateHelper(string table_name, string col_name) {
        Table* table = &database[table_name];
        auto it = find(table->col_names.begin(), table->col_names.end(), col_name);
        uint32_t col_index = static_cast<uint32_t>(it - table->col_names.begin());
        
        int temp_index = 0;
        if (table->index_type == Index::hash) {
            table->hash.clear();
            for (uint32_t i = 0; i < table->entries.size(); ++i) {
                table->hash[table->entries[i][col_index]].emplace_back(temp_index);
                temp_index++;
            }
        }
        else if (table->index_type == Index::bst) {
            table->bst.clear();
            for (uint32_t i = 0; i < table->entries.size(); ++i) {
                table->bst[table->entries[i][col_index]].emplace_back(temp_index);
                temp_index++;
            }
        }
    }
};

#endif /* simulation_h */
