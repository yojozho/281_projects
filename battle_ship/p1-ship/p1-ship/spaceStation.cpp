//Project identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A
//  stationSpace.cpp
//  p1-ship
//
//  Created by Joyce Zhou on 1/18/23.
//
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include "spaceStation.h"

using namespace std;

spaceStation::spaceStation() {
    num_level = 0;
    dim = 0;
    solution = false;
}

void spaceStation::getMode(int argc, char *argv[], Options& options) {
    int index = 0, choice = 0;
    
    opterr = false;
    
    option longOpts[] = {{"stack", no_argument, nullptr, 's'},
                                { "queue", no_argument, nullptr, 'q'},
                                { "output", required_argument, nullptr, 'o'},
                                { "help", no_argument, nullptr, 'h'},
                                { nullptr, 0, nullptr, '\0' }};
    
    while ((choice = getopt_long(argc, argv, "hsqo:", longOpts, &index)) != -1) {
        switch (choice)  {
            case 'h':
                cout << "Usage: " << argv[0] << "[-o M|L] | -h\n";
                cout << "This program is designed to help find a way out of the station,\n";
                cout << "-s to use stack routing scheme\n";
                cout << "-q to use queue routing scheme\n";
                cout << "-o (M|L) to format output file as map or coordinate list\n";
                cout << "-h for helpful message and exit program\n";
                exit(0);
                
            case 's':
                if (options.isStack || options.isQueue) {
                    cerr << "Multiple routing modes specified";
                    exit(1);
                }
                else
                    options.isStack = true;
                break;
                
            case 'q':
                if (options.isStack|| options.isQueue) {
                    cerr << "Multiple routing modes specified";
                    exit(1);
                }
                else
                    options.isQueue = true;
                break;
                
            case 'o': {
                string arg{optarg};
                switch(arg[0]) {
                    case 'M':
                        options.isMap = true;
                        break;
                    case 'L':
                        options.isList= true;
                        break;
                    default:
                        cerr << "Invalid output mode specified";
                        exit(1);
                        break;
                }//map or list
                break;
            } // case 'o'
            default:
                cerr << "Invalid option\n";
                exit(1);
        } //switch mode
    }
    
    if (!options.isStack && !options.isQueue) {
        cerr << "No routing mode specified";
        exit(1);
    }  // if ..mode
    
    if (!options.isMap && !options.isList) {
        options.isMap = true;
    } // if ..mode
} //getMode()

void spaceStation::getLayout() {
    char format, slash, input;
    cin >> format >> num_level >> dim;
    layout.resize(num_level, vector< vector<Status>>(dim, vector<Status>(dim)));
    string ignore;
    getline(cin, ignore);
    switch (format) {
        case 'M': {
            for (uint32_t i = 0; i < num_level; ++i) {
                for (uint32_t j = 0; j < dim; ++j) {
                    for (uint32_t k = 0; k < dim; ++k) {
                        cin >> input;
                        while (input =='/') {
                            getline(cin,ignore);
                            cin >> input;
                        }
                        if (!isValidLocation(input)) {
                            cerr << "Invalid map character";
                            exit(1);
                        }
                        layout[i][j][k].type = input;
                        if (layout[i][j][k].type == 'S') {
                            start.level = i;
                            start.row = j;
                            start.col = k;
                        }
                    }
                }
            } //fill layout
            break;
        } //case 'M'
            
        case 'L': {
            uint32_t l, r, c;
            while (cin >> slash) {
                if (slash == '/') {
                        getline(cin, ignore);
                }
                else if (slash == '(') {
                        cin >> l >> slash >> r >> slash >> c >> slash >> input;
                        if (l >= num_level) {
                            cerr << "Invalid map level";
                            exit(1);
                        }
                        if (r >= dim) {
                            cerr << "Invalid map row";
                            exit(1);
                        }
                        if (c >= dim) {
                            cerr << "Invalid map column";
                            exit(1);
                        }
                        if (!isValidLocation(input)) {
                            cerr << "Invalid map character";
                            exit(1);
                        }
                        layout[l][r][c].type = input;
                        if (layout[l][r][c].type == 'S') {
                            start.level = l;
                            start.row = r;
                            start.col = c;
                        }
                    cin >> slash;
                }
            }
        } //case 'L'
    } //end swtich
} //getLayout()

void spaceStation::readWithStack() {
    Coordinate current;
    container.push_back(start);
    layout[start.level][start.row][start.col].discovered = true;
    while (!container.empty() && !solution) {
        current = container.back();
        container.pop_back();
        if (current.row > 0){
            addToContainer(current.level, current.row - 1, current.col, 'n');
            if(layout[current.level][current.row - 1][current.col].type == 'H') {
                hanger.level = current.level;
                hanger.row = current.row - 1;
                hanger.col = current.col;
                solution = true;
                continue;
            }
        }
        //n
        if (current.col + 1 < dim) {
            addToContainer(current.level, current.row, current.col + 1, 'e');
            if(layout[current.level][current.row][current.col + 1].type == 'H') {
                hanger.level = current.level;
                hanger.row = current.row;
                hanger.col = current.col + 1;
                solution = true;
                continue;
            }
        }
        //e
        if (current.row + 1 < dim) {
            addToContainer(current.level, current.row + 1, current.col, 's');
            if(layout[current.level][current.row + 1][current.col].type == 'H') {
                hanger.level = current.level;
                hanger.row = current.row + 1;
                hanger.col = current.col;
                solution =  true;
                continue;
            }
        }
        //s
        if (current.col > 0) {
            addToContainer(current.level, current.row, current.col - 1, 'w');
            if(layout[current.level][current.row][current.col - 1].type == 'H') {
                hanger.level = current.level;
                hanger.row = current.row;
                hanger.col = current.col - 1;
                solution = true;
                continue;
            }
        }
        //w
        if (layout[current.level][current.row][current.col].type == 'E') {
            for (uint32_t i = 0; i < num_level; ++ i) {
                if ( i != current.level && layout[i][current.row][current.col].isDiscoverable() && layout[i][current.row][current.col].type == 'E') {
                    addToContainer(i, current.row, current.col, static_cast<char>(current.level + 48));
                }
            }
        } //check elevator
    } //while no hanger
    
}

void spaceStation::readWithQueue() {
    Coordinate current;
    container.push_back(start);
    layout[start.level][start.row][start.col].discovered = true;
    while (!container.empty() && !solution) {
        current = container.front();
        container.pop_front();
        if (current.row > 0){
            addToContainer(current.level, current.row - 1, current.col, 'n');
            if(layout[current.level][current.row - 1][current.col].type == 'H') {
                hanger.level = current.level;
                hanger.row = current.row - 1;
                hanger.col = current.col;
                solution = true;
                continue;
            }
        }
        //n
        if (current.col + 1 < dim) {
            addToContainer(current.level, current.row, current.col + 1, 'e');
            if(layout[current.level][current.row][current.col + 1].type == 'H') {
                hanger.level = current.level;
                hanger.row = current.row;
                hanger.col = current.col + 1;
                solution = true;
                continue;
            }
        }
        //e
        if (current.row + 1 < dim) {
            addToContainer(current.level, current.row + 1, current.col, 's');
            if(layout[current.level][current.row + 1][current.col].type == 'H') {
                hanger.level = current.level;
                hanger.row = current.row + 1;
                hanger.col = current.col;
                solution =  true;
                continue;
            }
        }
        //s
        if (current.col > 0) {
            addToContainer(current.level, current.row, current.col - 1, 'w');
            if(layout[current.level][current.row][current.col - 1].type == 'H') {
                hanger.level = current.level;
                hanger.row = current.row;
                hanger.col = current.col - 1;
                solution = true;
                continue;
            }
        }
        //w
        if (layout[current.level][current.row][current.col].type == 'E') {
            for (uint32_t i = 0; i < num_level; ++ i) {
                if ( i != current.level && layout[i][current.row][current.col].isDiscoverable() && layout[i][current.row][current.col].type == 'E') {
                    addToContainer(i, current.row, current.col, static_cast<char>(current.level + 48));
                }
            }
        } //check elevator
    } //while no hanger
}

//TODO: infinite loop with SpecE-M
void spaceStation::makeEscape() {
    bool isStart = false;
    Coordinate current = hanger;
    while (!isStart) {
        switch (layout[current.level][current.row][current.col].direction) {
            case 'n':
                current.row++;
                layout[current.level][current.row][current.col].type = 'n';
                escape.push_back(current);
                break;

            case 'e':
                current.col--;
                layout[current.level][current.row][current.col].type = 'e';
                escape.push_back(current);
                break;
                
            case 's':
                current.row--;
                layout[current.level][current.row][current.col].type = 's';
                escape.push_back(current);
                break;
                
            case 'w':
                current.col++;
                layout[current.level][current.row][current.col].type = 'w';
                escape.push_back(current);
                break;
            case '.':
                isStart = true;
                break;
            default:
                uint32_t new_lvl = static_cast<uint32_t>(layout[current.level][current.row][current.col].direction - 48);
                layout[new_lvl][current.row][current.col].type = static_cast<char>(current.level + 48);
                current.level = new_lvl;
                escape.push_back(current);
                break;
        }
    }
}

void spaceStation::addToContainer(uint32_t level, uint32_t row, uint32_t col, char direction) {
    if (layout[level][row][col].isDiscoverable()) {
        Coordinate investigate;
        layout[level][row][col].discovered = true;
        investigate.level = level;
        investigate.row = row;
        investigate.col = col;
        container.push_back(investigate);
        layout[level][row][col].direction = direction;
    }
}

void spaceStation::writeWithMap() {
    cout << "Start in level " << start.level << ", row " << start.row << ", column " << start.col << "\n";
    for (uint32_t i = 0; i < num_level; ++i) {
        cout << "//level " << i << "\n";
        for (uint32_t j = 0; j < dim; ++j) {
            for (uint32_t k = 0; k < dim; ++k) {
                cout << layout[i][j][k].type;
            }
            cout << "\n";
        }
    }
}

void spaceStation::writeWithList() {
    cout << "//path taken\n";
    while (!escape.empty()) {
        Coordinate current = escape.back();
        escape.pop_back();
        cout << "(" << current.level << "," << current.row << "," << current.col << ","  << layout[current.level][current.row][current.col].type << ")\n";
    }
}

bool spaceStation::getSolution() {
    return solution;
}
