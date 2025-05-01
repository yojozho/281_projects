//Project identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A
//  stationSpace.hpp
//  p1-ship
//
//  Created by Joyce Zhou on 1/18/23.
//

#ifndef spaceStation_h
#define spaceStation_h

#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>

using namespace std;

struct Options {
    bool isStack = false;
    bool isQueue = false;
    bool isMap = false;
    bool isList = false;
};

struct Status {
    char direction = '.', type = '.';
    bool discovered = false;
    
    bool isDiscoverable() {
        if (type != '#' && discovered == false) {
            return true;
        }
        return false;
    }
};

struct Coordinate {
    uint32_t level = 0, row = 0, col = 0;
};


class spaceStation {
public:
    spaceStation();
    void getMode(int argc, char *argv[], Options& options);
    void getLayout();
    void readWithStack();
    void readWithQueue();
    void makeEscape();
    void writeWithMap();
    void writeWithList();
    bool getSolution();
    
private:
    uint32_t num_level, dim;
    bool solution;
    vector< vector < vector<Status>>> layout;
    vector<Coordinate> escape;
    Coordinate start;
    Coordinate hanger;
    deque<Coordinate> container;
    void addToContainer(uint32_t level, uint32_t row, uint32_t col, char direction);
    bool isValidLocation(char c) {
        switch(c) {
            case '.':
            case '#':
            case 'S':
            case 'H':
            case 'E':
                return true;
                break;
            default:
                return false;
                break;
            
        }
    }
};

#endif /* spaceStation_h */
