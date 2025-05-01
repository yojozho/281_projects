//  Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0
//  simulation.cpp
//  p4-pokemon
//
//  Created by Joyce Zhou on 4/11/23.
//

#include <iostream>
#include <string>
#include <getopt.h>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "simulation.h"
using namespace std;

void Simulation::getOutputMode(int argc,char * argv[]) {
    int index = 0, choice = 0;
    opterr = false;
    option longOpts[] = {{ "help", no_argument, nullptr, 'h' },
                            { "mode", 1, nullptr, 'm' },
                            { nullptr, 0, nullptr, '\0' }};
    while ((choice = getopt_long(argc, argv, "hm:", longOpts, &index)) != -1) {
        switch(choice) {
            case 'h':
                cout << "This is a helpful message\n";
                exit(0);
                break;
            case 'm': {
                string arg{optarg};
                if (arg == "MST") {
                    mode = Mode::MST;
                }
                else if (arg == "FASTTSP") {
                    mode = Mode::FASTTSP;
                }
                else if (arg == "OPTTSP") {
                    mode = Mode::OPTTSP;
                }
                else {
                    cout << "Error: Invalid mode\n";
                    exit(1);
                }
                break;
            }
            default:
                cerr << "Error: Invalid command line option\n";
                exit(1);
        }
    }
}

void Simulation::processInput() {
    if (mode == Mode::MST) {
        PartA a;
        a.set_locations();
    }
    else if (mode == Mode::FASTTSP) {
        PartB b;
        b.set_coordinates();
    }
    else {
        PartC c;
        c.set_coordinates();
    }
}

void PartA::set_locations() {
    cin >> num_vertices;
    double x_coord, y_coord;
    uint32_t num_land = 0, num_sea = 0, num_coast = 0;
    Location temp;
    for (uint32_t i = 0; i < num_vertices; ++i) {
        cin >> x_coord >> y_coord;
        temp.x = x_coord;
        temp.y = y_coord;
        if (x_coord < 0 && y_coord < 0) {
            temp.type = Type::SEA;
            num_sea++;
        }
        else if ((x_coord <= 0 && y_coord == 0) || (x_coord == 0 && y_coord <= 0)) {
            temp.type = Type::COAST;
            num_coast++;
        }
        else {
            temp.type = Type::LAND;
            num_land++;
        }
        locations.push_back(temp);
    }
    if (num_land != 0 && num_sea !=0 && num_coast == 0) {
        cerr << "Cannot construct MST\n";
        exit(1);
    }
    makeMST();
    printMST();
}

void PartA::makeMST() {
    prim_table.resize(num_vertices);
    prim_table[0].dv = 0;
    prim_table[0].kv = true;
    for (uint32_t i = 0; i < num_vertices; ++i) {
        double min = INFINITY;
        uint32_t min_index = 0;
        for (uint32_t j = 0; j < num_vertices; ++j){
            Prim *v = &prim_table[j];
            if (v->kv ==  false && v->dv < min) {
                min = v->dv;
                min_index = j;
            }
        }//find v
        prim_table[min_index].kv = true;
        
        for (uint32_t k = 0; k < num_vertices; ++k) {
            Prim *w = &prim_table[k];
            if (w->kv == false && isValidConnection(locations[min_index], locations[k])) {
                double temp_dist = calculateDistance(locations[min_index], locations[k]);
                if (temp_dist < w->dv) {
                    w->dv = temp_dist;
                    w->pv = min_index;
                }
            }
        }//find w
    }
}

void PartA::printMST() {
    double total_weight = 0;
    for (uint32_t i = 0; i < num_vertices; ++i) {
        total_weight += sqrt(prim_table[i].dv);
    }
    cout << total_weight << "\n";
    
    for (uint32_t j = 1; j < num_vertices; ++j) {
        if (prim_table[j].pv < j) {
            cout << prim_table[j].pv << " " << j << "\n";
        }
        else {
            cout << j << " " << prim_table[j].pv << "\n";
        }
    }
}

void PartB::set_coordinates() {
    cin >> num_coordinates;
    double x_coord, y_coord;
    Coordinates temp;
    for (uint32_t i = 0; i < num_coordinates; ++i) {
        cin >> x_coord >> y_coord;
        temp.x = x_coord;
        temp.y = y_coord;
        coordinates.push_back(temp);
    }
    makeFASTTSP();
    printFASTTSP();
}

void PartB::makeFASTTSP() {
    path.push_back(0);
    path.push_back(1);
    path.push_back(0);
    for (uint32_t k = 2; k < num_coordinates; ++k) {
        double min = INFINITY;
        uint32_t j_index = 0;
        for (uint32_t i = 0; i < path.size() - 1; ++i) {
            double cost = calculateCost(path[i], path[i + 1], k);
            if (cost < min) {
                min = cost;
                j_index = i + 1;
            }
        }
        path.insert(path.begin() + j_index, k);
    }
}

void PartB::printFASTTSP() {
    double distance = 0;
    for (uint32_t i = 0; i < num_coordinates; ++i) {
        distance += sqrt(calculateDistance(coordinates[path[i]], coordinates[path[i + 1]]));
    }
    cout << distance << "\n";
    
    for (uint32_t j = 0; j < num_coordinates; ++j) {
        cout << path[j] << " ";
    }
}

void PartC::set_coordinates() {
    cin >> num_coordinates;
    double x_coord, y_coord;
    Coordinates temp;
    for (uint32_t i = 0; i < num_coordinates; ++i) {
        cin >> x_coord >> y_coord;
        temp.x = x_coord;
        temp.y = y_coord;
        coordinates.push_back(temp);
    }
    makeOPTTSP();
    printOPTTSP();
}


void PartC::makeOPTTSP() {
    moddedFASTTSP();
    current_path = best_path;
    genPerms(1);
}

void PartC::printOPTTSP() {
    cout << best_distance << "\n";
    
    for (uint32_t i = 0; i < num_coordinates; ++i) {
        cout << best_path[i] << " ";
    }
}

void PartC::genPerms(uint32_t perm_length) {
    if (perm_length == current_path.size() - 1) {
        double temp_dist = calculateDistance(coordinates[current_path[0]], coordinates[current_path[perm_length - 1]]);
        current_distance += temp_dist;
        if (current_distance < best_distance) {
            best_path = current_path;
            best_distance = current_distance;
        }
        current_distance -= temp_dist;
        return;
  }
    
  if (!promising(perm_length)) {
      return;
  }
    
  for (size_t i = perm_length; i < current_path.size() - 1; ++i) {
      swap(current_path[perm_length], current_path[i]);
      double temp_dist = calculateDistance(coordinates[current_path[perm_length - 1]], coordinates[current_path[perm_length]]);
      current_distance += temp_dist;
      genPerms(perm_length + 1);
      current_distance -= temp_dist;
      swap(current_path[perm_length], current_path[i]);
  }
}

bool PartC::promising(uint32_t perm_length) {
    if (current_path.size() - perm_length < 5) {
        return true;
    }
    unvisited.clear();
    unvisited.reserve(num_coordinates - perm_length);
    for (uint32_t i = perm_length; i < num_coordinates; ++i) {
        unvisited.push_back(current_path[i]);
    }
    
    vector<Prim> prim_table = moddedMST(unvisited);
    
    double lower_bound = calculateLowerbound(prim_table, perm_length);
    
    if (lower_bound < best_distance) {
        return true;
    }
    return false;
}

void PartC::moddedFASTTSP() {
    best_path.push_back(0);
    best_path.push_back(1);
    best_path.push_back(0);
    for (uint32_t k = 2; k < num_coordinates; ++k) {
        double min = INFINITY;
        uint32_t j_index = 0;
        for (uint32_t i = 0; i < best_path.size() - 1; ++i) {
            double cost = calculateCost(best_path[i], best_path[i + 1], k);
            if (cost < min) {
                min = cost;
                j_index = i + 1;
            }
        }
        best_path.insert(best_path.begin() + j_index, k);
    }
    
    for (uint32_t i = 0; i < num_coordinates; ++i) {
        best_distance += calculateDistance(coordinates[best_path[i]], coordinates[best_path[i + 1]]);
    }
}

vector<Prim> PartC::moddedMST(vector<uint32_t> &unvisited) {
    vector<Prim> prim_table;
    prim_table.resize(unvisited.size());
    prim_table[0].dv = 0;
    prim_table[0].kv = true;
    for (uint32_t i = 0; i < unvisited.size(); ++i) {
        double min = INFINITY;
        uint32_t min_index = 0;
        for (uint32_t j = 0; j < unvisited.size(); ++j){
            Prim *v = &prim_table[j];
            if (v->kv ==  false && v->dv < min) {
                min = v->dv;
                min_index = j;
            }
        }//find v
        prim_table[min_index].kv = true;
        
        for (uint32_t k = 0; k < unvisited.size(); ++k) {
            Prim *w = &prim_table[k];
            if (w->kv == false) {
                double temp_dist = calculateDistance(coordinates[unvisited[min_index]], coordinates[unvisited[k]]);
                if (temp_dist < w->dv) {
                    w->dv = temp_dist;
                    w->pv = min_index;
                }
            }
        }//find w
    }
    return prim_table;
}



