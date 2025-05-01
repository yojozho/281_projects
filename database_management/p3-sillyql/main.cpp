// Project Identifier: C0F4DFE8B340D81183C208F70F9D2D797908754D
//  main.cpp
//  p3-sillyql
//
//  Created by Joyce Zhou on 3/14/23.
//

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <getopt.h>
#include "TableEntry.h"
#include "simulation.h"

using namespace std;

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    
    cin >> boolalpha;
    cout << boolalpha;
    
    Simulation simulation;
    simulation.getOutputMode(argc, argv);
    simulation.processInput();
    
}
