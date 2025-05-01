//Project identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A
//  main.cpp
//  p1-ship
//
//  Created by Joyce Zhou on 1/17/23.
//
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <deque>
#include "spaceStation.h"

using namespace std;

int main (int argc, char *argv[]) {
    
    ios_base::sync_with_stdio(false);
    
    Options options;
    spaceStation station;
    station.getMode(argc, argv, options);
    station.getLayout();
    
    if (options.isStack)
        station.readWithStack();
    else if (options.isQueue)
        station.readWithQueue();
    
    if (station.getSolution())
        station.makeEscape();
    
    if (options.isMap)
        station.writeWithMap();
    else if (options.isList)
        station.writeWithList();
    
    return 0;
}
