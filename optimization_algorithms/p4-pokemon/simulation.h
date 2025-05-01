//  Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0
//  simulation.hpp
//  p4-pokemon
//
//  Created by Joyce Zhou on 4/11/23.
//

#ifndef simulation_h
#define simulation_h

#include <iostream>
#include <string>
#include <getopt.h>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

enum Type {
    LAND, SEA, COAST
};

enum Mode : uint32_t {
    NONE, MST, FASTTSP, OPTTSP
};

struct Location {
    double x;
    double y;
    Type type;
};

struct Prim {
    bool kv = false;
    double dv = INFINITY;
    uint32_t pv;
};

struct Coordinates {
    double x;
    double y;
};

class PartA {
public:
    void set_locations();
    void makeMST();
    void printMST();
private:
    vector<Location> locations;
    uint32_t num_vertices = 0;
    vector<Prim> prim_table;
    
    double calculateDistance(Location &l1, Location &l2) {
        return ((l1.x - l2.x) * (l1.x - l2.x)) + ((l1.y - l2.y) * (l1.y - l2.y));
    }
    
    bool isValidConnection(Location &l1, Location &l2) {
        if ((l1.type == Type::LAND && l2.type == Type::SEA) || (l1.type == Type::SEA && l2.type == Type::LAND)) {
            return false;
        }
        return true;
    }
    
};

class PartB {
public:
    void set_coordinates();
    void makeFASTTSP();
    void printFASTTSP();
private:
    vector<Coordinates> coordinates;
    uint32_t num_coordinates;
    vector<uint32_t> path;
    
    double calculateDistance(Coordinates &c1, Coordinates &c2) {
        return ((c1.x - c2.x) * (c1.x - c2.x)) + ((c1.y - c2.y) * (c1.y - c2.y));
    }
    
    double calculateCost(uint32_t i, uint32_t j, uint32_t k) {
        double cik = sqrt(calculateDistance(coordinates[i], coordinates[k])),
        ckj = sqrt(calculateDistance(coordinates[k], coordinates[j])),
        cij = sqrt(calculateDistance(coordinates[i], coordinates[j]));
        double cost = cik + ckj - cij;
        return cost;
    }
};

class PartC {
public:
    void set_coordinates();
    void makeOPTTSP();
    void printOPTTSP();
    vector<Prim> moddedMST(vector<uint32_t> &unvisited);
    void moddedFASTTSP();
    void genPerms(uint32_t permLength);
    bool promising(uint32_t permLength);
private:
    vector<Coordinates> coordinates;
    uint32_t num_coordinates = 0;
    vector<uint32_t> unvisited;
    vector<uint32_t> current_path;
    double current_distance = 0;
    vector<uint32_t> best_path;
    double best_distance = 0;
    
    double calculateDistance(Coordinates &c1, Coordinates &c2) {
        return sqrt(((c1.x - c2.x) * (c1.x - c2.x)) + ((c1.y - c2.y) * (c1.y - c2.y)));
    }
    
    double calculateCost(uint32_t i, uint32_t j, uint32_t k) {
        double cik = calculateDistance(coordinates[i], coordinates[k]),
        ckj = calculateDistance(coordinates[k], coordinates[j]),
        cij = calculateDistance(coordinates[i], coordinates[j]);
        double cost = cik + ckj - cij;
        return cost;
    }
    
    double calculateLowerbound(vector<Prim> &prim_table, uint32_t perm_length) {
        double unvisited_total = 0;
        for (uint32_t i = 0; i < prim_table.size(); ++i) {
            unvisited_total += prim_table[i].dv;
        }
        
        double start_min = INFINITY, end_min = INFINITY;
        for (uint32_t i = 0; i < unvisited.size(); ++i) {
            double arm1 = calculateDistance(coordinates[current_path[0]], coordinates[unvisited[i]]);
            double arm2 = calculateDistance(coordinates[current_path[perm_length - 1]], coordinates[unvisited[i]]);
            if (arm1 < start_min) {
                start_min = arm1;
            }
            if (arm2 < end_min) {
                end_min = arm2;
            }
            
        }
        double lower_bound = current_distance + unvisited_total + start_min + end_min;
        return lower_bound;
    }
    
};

class Simulation {
public:
    void getOutputMode(int argc, char * argv[]);
    void processInput();
private:
    Mode mode = Mode::NONE;
};


#endif /* simulation_h */
