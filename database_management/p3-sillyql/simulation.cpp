// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
//  simulation.cpp
//  p3-sillyql
//
//  Created by Joyce Zhou on 3/21/23.
//

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <getopt.h>
#include "simulation.h"
using namespace std;

void Simulation::getOutputMode(int argc,char * argv[]) {
    int index = 0, choice = 0;
    opterr = false;
    option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                            { "quiet", no_argument, nullptr, 'q' },
                            { nullptr, 0, nullptr, '\0' }};
    while ((choice = getopt_long(argc, argv, "hq", longOpts, &index)) != -1) {
        switch(choice) {
            case 'h':
                cout << "This is a helpful message\n";
                exit(0);
                break;
            case 'q':
                quietMode = true;
                break;
            default:
                cerr << "Error: Unknown command line option\n";
                exit(1);
        }
    }
}

void Simulation::processInput() {
    string cmd;
    do {
        string ignore, table_name;
        cout << "% ";
        cin >> cmd;
        if (cmd[0] == '#') {
            getline(cin, ignore);
        }
        else if (cmd == "CREATE") {
            cin >> table_name;
            if (database.find(table_name) != database.end()) {
                cout << "Error during CREATE: Cannot create already existing table " << table_name << "\n";
                getline(cin, ignore);
            }
            else {
                createTable(table_name);
            }
        }
        else if (cmd == "REMOVE") {
            cin >> table_name;
            if (database.find(table_name) == database.end()) {
                cout << "Error during REMOVE: " << table_name << " does not name a table in the database\n";
            }
            else {
                database.erase(database.find(table_name));
                cout << "Table " << table_name << " deleted\n";
            }
        }
        else if (cmd == "INSERT") {
            uint32_t add_rows;
            cin >> ignore >> table_name >> add_rows >> ignore;
            if (database.find(table_name) == database.end()){
                cout << "Error during INSERT: " << table_name << " does not name a table in the database\n";
                getline(cin, ignore);
            }
            else {
                insertEntry(table_name, add_rows);
            }
        }
        else if (cmd == "PRINT") {
            cin >> ignore >> table_name;
            if (database.find(table_name) == database.end()) {
                cout << "Error during PRINT: " << table_name << " does not name a table in the database\n";
                getline(cin, ignore);
            }
            else {
                printEntries(table_name);
            }
        }
        else if (cmd == "DELETE") {
            cin >> ignore >> table_name >> ignore;
            if (database.find(table_name) == database.end()) {
                cout << "Error during DELETE: " << table_name << " does not name a table in the database\n";
                getline(cin, ignore);
            }
            else {
                deleteEntries(table_name);
            }
        }
        else if (cmd == "JOIN") {
            string table_name1, table_name2;
            cin >> table_name1 >> ignore >> table_name2 >> ignore;
            
            if (database.find(table_name1) == database.end()) {
                cout << "Error during JOIN: " << table_name1 << " does not name a table in the database\n";
                getline(cin, ignore);
            }
            else if (database.find(table_name2) == database.end()) {
                cout << "Error during JOIN: " << table_name2 << " does not name a table in the database\n";
                getline(cin, ignore);
            }
            else {
                joinTables(table_name1, table_name2);
            }
        }
        else if (cmd == "GENERATE") {
            cin >> ignore >> table_name;
            if (database.find(table_name) == database.end()) {
                cout << "Error during GENERATE: " << table_name << " does not name a table in the database\n";
                getline(cin, ignore);
            }
            else {
                generateIndex(table_name);
            }
        }
        else if (cmd != "QUIT") {
            cout << "Error: unrecognized command\n";
            getline(cin, ignore);
        }
    } while (cmd != "QUIT");
    
    cout << "Thanks for being silly!\n";
    
}

void Simulation::createTable(string table_name) {
    Table new_table;
    cin >> new_table.num_cols;
    for (uint32_t i = 0; i < new_table.num_cols; ++i) {
        string type;
        cin >> type;
        if (type == "string") {
            new_table.col_types.push_back(EntryType::String);
        }
        else if (type == "double") {
            new_table.col_types.push_back(EntryType::Double);
        }
        else if (type == "int") {
            new_table.col_types.push_back(EntryType::Int);
        }
        else {
            new_table.col_types.push_back(EntryType::Bool);
        }
    }
    
    cout << "New table " << table_name << " with column(s) ";
    for (uint32_t j = 0; j < new_table.num_cols; ++j) {
        string name;
        cin >> name;
        new_table.col_names.push_back(name);
        cout << name << " ";
    }
    cout << "created\n";
    
    pair<string, Table> new_pair{table_name, new_table};
    database.insert(new_pair);
}

void Simulation::insertEntry(string table_name, uint32_t add_rows) {
    Table* table = &database[table_name];
    uint32_t start_pos = static_cast<uint32_t>(table->entries.size()),
                            end_pos = (start_pos + add_rows) - 1;
    table->entries.resize(start_pos + add_rows);
    
    for (uint32_t k = start_pos; k <= end_pos; ++k) {
        table->entries[k].reserve(table->num_cols);
    }
    for (uint32_t i = start_pos; i <= end_pos; ++i) {
        for (uint32_t j = 0; j < table->num_cols; ++j) {
            if (table->col_types[j] == EntryType::String) {
                string input;
                cin >> input;
                TableEntry new_entry = TableEntry(input);
                table->entries[i].emplace_back(new_entry);
            }
            else if (table->col_types[j] == EntryType::Double) {
                double input;
                cin >> input;
                TableEntry new_entry = TableEntry(input);
                table->entries[i].emplace_back(new_entry);
            }
            else if (table->col_types[j] == EntryType::Int) {
                int input;
                cin >> input;
                TableEntry new_entry = TableEntry(input);
                table->entries[i].emplace_back(new_entry);
            }
            else {
                bool input;
                cin >> input;
                TableEntry new_entry = TableEntry(input);
                table->entries[i].emplace_back(new_entry);
            }
        }
    }
    cout << "Added " << add_rows << " rows to " << table_name << " from position "
         << start_pos << " to " << end_pos << "\n";
    
    if (table->col_hash != "" ) {
        regenerateHelper(table_name, table->col_hash);
    }
}

void Simulation::printEntries(string table_name) {
    Table* table = &database[table_name];
    string col_name, ignore, option;
    vector<uint32_t> cols_to_print;
    uint32_t num_cols_to_print = 0;
    cin >> num_cols_to_print;
    for (uint32_t i = 0; i < num_cols_to_print; ++i) {
        cin >> col_name;
        auto it = find(table->col_names.begin(), table->col_names.end(), col_name);
        if (it == table->col_names.end()) {
            cout << "Error during PRINT: " << col_name << " does not name a column in " << table_name << "\n";
            getline(cin, ignore);
            return;
        }
        uint32_t index = static_cast<uint32_t>(it - table->col_names.begin());
        cols_to_print.push_back(index);
    }
    
    cin >> option;
    if (option == "ALL") {
        if (!quietMode) {
            for (uint32_t i = 0; i < cols_to_print.size(); ++i) {
                cout << table->col_names[cols_to_print[i]] << " ";
            }
            cout << "\n";
            
            for (uint32_t i = 0; i < table->entries.size(); ++i) {
                for (uint32_t j = 0; j < num_cols_to_print; ++j) {
                    cout << table->entries[i][cols_to_print[j]] << " ";
                }
                cout << "\n";
            }
        }//if quiet
        cout << "Printed " << table->entries.size() << " matching rows from " << table_name << "\n";
    }
    else {
        string col_condition;
        char symbol;
        cin >> col_condition >> symbol;
        auto it = find(table->col_names.begin(), table->col_names.end(), col_condition);
        if (it == table->col_names.end()) {
            cout << "Error during PRINT: " << col_name << " does not name a column in " << table_name << "\n";
            getline(cin, ignore);
            return;
        }
        
        if (!quietMode) {
            for (uint32_t i = 0; i < cols_to_print.size(); ++i) {
                cout << table->col_names[cols_to_print[i]] << " ";
            }
            cout << "\n";
        }
        
        uint32_t col_condition_index = static_cast<uint32_t>(it - table->col_names.begin());
        TableEntry entry_condition = deleteHelper(table_name, col_condition_index);
         
        uint32_t num_rows_printed = 0;
         if (table->index_type == Index::bst && table->col_hash == col_condition){
             if (symbol == '<') {
                 for (auto i = table->bst.begin(); i != table->bst.lower_bound(entry_condition); ++i) {
                     num_rows_printed += static_cast<uint32_t>(i->second.size());
                     if (!quietMode) {
                         for (uint32_t j = 0; j < i->second.size(); ++j) {
                             for (uint32_t k = 0; k < cols_to_print.size(); ++k) {
                                 cout << table->entries[i->second[j]][cols_to_print[k]] << " ";
                             }
                             cout << "\n";
                         }
                     }
                 }
             }
             else if (symbol == '>') {
                 for (auto i = table->bst.upper_bound(entry_condition); i != table->bst.end(); ++i) {
                     num_rows_printed += static_cast<uint32_t>(i->second.size());
                     if (!quietMode) {
                         for (uint32_t j = 0; j < i->second.size(); ++j) {
                             for (uint32_t k = 0; k < cols_to_print.size(); ++k) {
                                 cout << table->entries[i->second[j]][cols_to_print[k]] << " ";
                             }
                             cout << "\n";
                         }
                     }
                 }
             }
             else {
                 auto it = table->bst.find(entry_condition);
                 if (it != table->bst.end()) {
                     num_rows_printed += static_cast<uint32_t>(it->second.size());
                     if (!quietMode) {
                         for (uint32_t i = 0; i < it->second.size(); ++i) {
                             for (uint32_t j = 0; j < num_cols_to_print; ++j) {
                                 cout << table->entries[it->second[i]][cols_to_print[j]] << " ";
                             }
                             cout << "\n";
                         }
                     }
                 }
             }
             
         }
         else if (table->index_type == Index::hash && table->col_hash == col_condition
                  && symbol == '=') {
             auto it = table->hash.find(entry_condition);
             if (it != table->hash.end()) {
                 num_rows_printed += static_cast<uint32_t>(it->second.size());
                 if (!quietMode){
                     for (uint32_t i = 0; i < it->second.size(); ++i) {
                        for (uint32_t j = 0; j < num_cols_to_print; ++j) {
                            cout << table->entries[it->second[i]][cols_to_print[j]] << " ";
                        }
                         cout << "\n";
                    }
                 }
             }
         }
         else {
             if (symbol == '=') {
                 for (uint32_t i = 0; i < table->entries.size(); ++i) {
                     if (table->entries[i][col_condition_index] == entry_condition) {
                         if (!quietMode) {
                             for (uint32_t j = 0; j < num_cols_to_print; ++j) {
                                cout << table->entries[i][cols_to_print[j]] << " ";
                             }
                             cout << "\n";
                         }
                         num_rows_printed++;
                     }
                 }
             }
             else if (symbol == '<' ) {
                 for (uint32_t i = 0; i < table->entries.size(); ++i) {
                     if (table->entries[i][col_condition_index] < entry_condition) {
                         if (!quietMode) {
                             for (uint32_t j = 0; j < num_cols_to_print; ++j) {
                                 cout << table->entries[i][cols_to_print[j]] << " ";
                             }
                             cout << "\n";
                         }
                         num_rows_printed++;
                     }
                 }
             }
             else {
                 for (uint32_t i = 0; i < table->entries.size(); ++i) {
                     if (table->entries[i][col_condition_index] > entry_condition) {
                         if (!quietMode) {
                             for (uint32_t j = 0; j < num_cols_to_print; ++j) {
                                cout << table->entries[i][cols_to_print[j]] << " ";
                             }
                             cout << "\n";
                         }
                         num_rows_printed++;
                     }
                 }
             }
         }
        
        cout << "Printed " << num_rows_printed << " matching rows from " << table_name << "\n";
    }
}

void Simulation::deleteEntries(string table_name) {
    Table* table = &database[table_name];
    string col_name;
    char symbol;
    cin >> col_name >> symbol;
    auto it = find(table->col_names.begin(), table->col_names.end(), col_name);
    
    if (it == table->col_names.end()) {
        string ignore;
        cout << "Error during DELETE: " << col_name << " does not name a column in " << table_name << "\n";
        getline(cin, ignore);
        return;
    }
    
    uint32_t og_size = static_cast<uint32_t>(table->entries.size());
    
    uint32_t index = static_cast<uint32_t>(it - table->col_names.begin());
    
    TableEntry temp_entry = deleteHelper(table_name, index);
    
    if (symbol == '>') {
        table->entries.erase(remove_if
                                        (table->entries.begin(), table->entries.end(),
                                            greaterThan(index, temp_entry)), table->entries.end());
    }
    else if (symbol == '<') {
        table->entries.erase(remove_if
                                        (table->entries.begin(), table->entries.end(),
                                            lessThan(index, temp_entry)), table->entries.end());
    }
    else {
        table->entries.erase(remove_if
                                        (table->entries.begin(), table->entries.end(),
                                            equalTo(index, temp_entry)), table->entries.end());
    }
    
    
    uint32_t deleted_rows = og_size - static_cast<uint32_t>(table->entries.size());
    cout << "Deleted " << deleted_rows << " rows from " << table_name << "\n";
    
    if (table->col_hash != "" ) {
        regenerateHelper(table_name, table->col_hash);
    }
}

void Simulation::generateIndex(string table_name) {
    Table* table = &database[table_name];
    string ignore, index_type, col_name;
    
    cin >> index_type >> ignore >> ignore >> col_name;
    table->col_hash = col_name;
    
    auto it = find(table->col_names.begin(), table->col_names.end(), col_name);
    
    if (it == table->col_names.end()) {
        string ignore;
        cout << "Error during GENERATE: " << col_name << " does not name a column in " << table_name << "\n";
        getline(cin, ignore);
        return;
    }
    
    uint32_t col_index = static_cast<uint32_t>(it - table->col_names.begin());
    
    if (table->index_type == Index::hash) {
        table->hash.clear();
    }
    if (table->index_type == Index::bst) {
        table->bst.clear();
    }
    
    int temp_index = 0;
    if (index_type == "hash") {
        table->index_type = Index::hash;
        for (uint32_t i = 0; i < table->entries.size(); ++i) {
            table->hash[table->entries[i][col_index]].emplace_back(temp_index);
            temp_index++;
        }
    }
    else {
        table->index_type = Index::bst;
        for (uint32_t i = 0; i < table->entries.size(); ++i) {
            table->bst[table->entries[i][col_index]].emplace_back(temp_index);
            temp_index++;
        }
    }
    
    cout << "Created " << index_type << " index for table " << table_name << " on column " << col_name << "\n";
}

void Simulation::joinTables(string table_name1, string table_name2) {
    Table *table1 = &database[table_name1];
    Table *table2 = &database[table_name2];
    string col_name1, col_name2, ignore;
    vector<string> print_colnames;
    vector<uint32_t> col_table_num;
    uint32_t num_cols_to_print;
    char symbol;
    cin >> col_name1 >> symbol >> col_name2 >> ignore >> ignore >> num_cols_to_print;
    print_colnames.reserve(num_cols_to_print);
    col_table_num.reserve(num_cols_to_print);
    
    auto it1 = find(table1->col_names.begin(), table1->col_names.end(), col_name1);
    auto it2 = find(table2->col_names.begin(), table2->col_names.end(), col_name2);
    
    if (it1 == table1->col_names.end()) {
        cout << "Error during JOIN: " << col_name1 << " does not name a column in " << table_name1 << "\n";
        getline(cin, ignore);
        return;
    }
    if (it2 == table2->col_names.end()) {
        cout << "Error during JOIN: " << col_name2 << " does not name a column in " << table_name2 << "\n";
        getline(cin, ignore);
        return;
    }
    
    string temp_col_name;
    uint32_t temp_table_num;
    for (uint32_t i = 0; i < num_cols_to_print; ++i) {
        cin >> temp_col_name >> temp_table_num;
        if (temp_table_num == 1) {
            auto temp_it = find(table1->col_names.begin(), table1->col_names.end(), temp_col_name);
            if (temp_it == table1->col_names.end()) {
                cout << "Error during JOIN: " << temp_col_name << " does not name a column in " << table_name1 << "\n";
                getline(cin, ignore);
                return;
            }
        }
        else {
            auto temp_it = find(table2->col_names.begin(), table2->col_names.end(), temp_col_name);
            if (temp_it == table2->col_names.end()) {
                cout << "Error during JOIN: " << temp_col_name << " does not name a column in " << table_name2 << "\n";
                getline(cin, ignore);
                return;
            }
        }
        print_colnames.push_back(temp_col_name);
        col_table_num.push_back(temp_table_num);
    }
    
    unordered_map<TableEntry, vector<uint32_t>> table2_hash;
    uint32_t table1_col_index = static_cast<uint32_t>(it1 - table1->col_names.begin());
    uint32_t table2_col_index = static_cast<uint32_t>(it2 - table2->col_names.begin());
    uint32_t hash_index = 0;
    for (uint32_t j = 0; j < table2->entries.size(); ++j) {
        table2_hash[table2->entries[j][table2_col_index]].emplace_back(hash_index);
        hash_index++;
    }
    
    if (!quietMode) {
        for (uint32_t k = 0; k < print_colnames.size(); ++k) {
            cout << print_colnames[k] << " ";
        }
        cout << "\n";
    }
    
    uint32_t num_rows_printed = 0;
    for (uint32_t i = 0; i < table1->entries.size(); ++i) {
        auto it = table2_hash.find(table1->entries[i][table1_col_index]);
        if (it != table2_hash.end()) {
            for (uint32_t j = 0; j < it->second.size(); ++j) {
                if (!quietMode) {
                    for (uint32_t k = 0; k < print_colnames.size(); ++k) {
                        if (col_table_num[k] == 1) {
                            auto find_col = find(table1->col_names.begin(), table1->col_names.end(), print_colnames[k]);
                            uint32_t col_index = static_cast<uint32_t>(find_col - table1->col_names.begin());
                            cout << table1->entries[i][col_index] << " ";
                        }
                        else {
                            auto find_col = find(table2->col_names.begin(), table2->col_names.end(), print_colnames[k]);
                            uint32_t col_index = static_cast<uint32_t>(find_col - table2->col_names.begin());
                            cout << table2->entries[it->second[j]][col_index] << " ";
                        }
                    }
                    cout << "\n";
                }
                num_rows_printed++;
            }
        }
    }
    
    cout << "Printed " << num_rows_printed << " rows from joining " << table_name1 << " to " << table_name2 << "\n";
}
