//  Project Identifier: 5949F553E20B650AB0FB2266D3C0822B13D248B0
//  main.cpp
//  p4-pokemon
//
//  Created by Joyce Zhou on 4/10/23.
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

int main(int argc, char * argv[]) {
    ios_base::sync_with_stdio(false);
    cout << setprecision(2);
    cout << fixed;
    Simulation simulation;
    simulation.getOutputMode(argc, argv);
    simulation.processInput();
    return 0;
}
