//
//  main.cpp
//  p2-a
//
//  Created by Joyce Zhou on 2/14/23.
// AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include <getopt.h>
#include <vector>
#include <queue>
#include <iostream>
#include <sstream>
#include "simulation.h"
using namespace std;

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(false);

    Options options;
    Simulation simulation;
    simulation.getOutputMode(argc, argv, options);
    cout << "Deploying troops...\n";
    simulation.deployBattalions(options);
}
