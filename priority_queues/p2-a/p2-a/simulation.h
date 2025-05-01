//
//  simulation.hpp
//  p2-a
//
//  Created by Joyce Zhou on 2/15/23.
//AD48FB4835AF347EB0CA8009E24C3B13F8519882

#ifndef simulation_h
#define simulation_h

#include <getopt.h>
#include <vector>
#include <queue>
#include <iostream>
#include <sstream>
using namespace std;

struct Options {
    bool verbose = false;
    bool median = false;
    bool general = false;
    bool watcher = false;
};

struct Battalion {
    Battalion();
    uint32_t general_id;
    uint32_t planet_id;
    uint32_t force_sense;
    mutable uint32_t num_troops;
    uint32_t priority_id;
    Battalion(uint32_t general_id, uint32_t planet_id, uint32_t force_sense,
              uint32_t num_troops, uint32_t priority_id) : general_id(general_id),
                                            planet_id(planet_id),
                                            force_sense(force_sense),
                                            num_troops(num_troops),
                                            priority_id(priority_id) {}
};

struct General {
    uint32_t num_jedi = 0;
    uint32_t num_sith = 0;
    uint32_t num_losses = 0;
};

enum class State {Zero, One, Two};

struct AmbushNotebook {
    AmbushNotebook();
    uint32_t weakest_jedi = 0, jedi_timestamp;
    uint32_t strongest_sith = 0, sith_timestamp;
    uint32_t maybe_best_sith = 0, maybe_sith_timestamp;
    uint32_t greatest_difference = 0;
    State state = State::Zero;
};

struct AttackNotebook {
    AttackNotebook();
    uint32_t weakest_jedi = 0, jedi_timestamp;
    uint32_t strongest_sith = 0, sith_timestamp;
    uint32_t maybe_best_jedi = 0, maybe_jedi_timestamp;
    uint32_t greatest_difference = 0;
    State state = State::Zero;
};

struct jediComparator {
    bool operator() (const Battalion &one, const Battalion &two) {
        if (one.force_sense != two.force_sense) {
            return one.force_sense > two.force_sense;
        }
        else {
            return one.priority_id > two.priority_id;
        }
    }
};

struct sithComparator {
    bool operator() (const Battalion &one, const Battalion &two) {
        if (one.force_sense != two.force_sense) {
            return one.force_sense < two.force_sense;
        }
        else {
            return one.priority_id > two.priority_id;
        }
    }
};

class Planet {
public:
    Planet();
    void jediInput(Battalion &b, Options &options, vector<General> &generals, uint32_t &battles);
    void sithInput(Battalion &b, Options &options, vector<General> &generals, uint32_t &battles);
    uint32_t calculateMedian();
    void jediUpdateNotebook(uint32_t force_sense, uint32_t timestamp);
    void sithUpdateNotebook(uint32_t force_sense, uint32_t timestamp);
    void printAmbushAttack(uint32_t planet_id);
    
private:
    priority_queue<Battalion, vector<Battalion>, jediComparator> jedi;
    priority_queue<Battalion, vector<Battalion>, sithComparator> sith;
    priority_queue<uint32_t, vector<uint32_t>, greater<uint32_t>> minPQ;
    priority_queue<uint32_t> maxPQ;
    AmbushNotebook ambush;
    AttackNotebook attack;
    
    uint32_t sithAttack(Options &options, vector<General> &generals) {
        uint32_t deaths;
        if (sith.top().num_troops == jedi.top().num_troops) {
            deaths = sith.top().num_troops + jedi.top().num_troops;
            if (options.general == true) {
                generals[jedi.top().general_id].num_losses += deaths / 2;
                generals[sith.top().general_id].num_losses += deaths / 2;
            }//if ..general
            jedi.pop();
            sith.pop();
        }
        else if (sith.top().num_troops > jedi.top().num_troops) {
            deaths = jedi.top().num_troops * 2;
            if (options.general == true) {
                generals[jedi.top().general_id].num_losses += deaths / 2;
                generals[sith.top().general_id].num_losses += deaths / 2;
            }//if ..general
            sith.top().num_troops -= jedi.top().num_troops;
            jedi.pop();
        }
        else {
            deaths = sith.top().num_troops * 2;
            if (options.general == true) {
                generals[jedi.top().general_id].num_losses += deaths / 2;
                generals[sith.top().general_id].num_losses += deaths / 2;
            }//if ..general
            jedi.top().num_troops -= sith.top().num_troops;
            sith.pop();
            
        }//if ..deaths
        
        if (options.median == true) {
            if (minPQ.empty() && maxPQ.empty()) {
                maxPQ.push(deaths);
            }
            else if (deaths >= maxPQ.top()){
                minPQ.push(deaths);
                if (minPQ.size() > maxPQ.size() && minPQ.size() - maxPQ.size() >= 2) {
                    maxPQ.push(minPQ.top());
                    minPQ.pop();
                }
            }
            else {
                maxPQ.push(deaths);
                if (maxPQ.size() > minPQ.size() && maxPQ.size() - minPQ.size() >= 2) {
                    minPQ.push(maxPQ.top());
                    maxPQ.pop();
                }
            }
        }//if ..median
        return deaths;
    }
};

class Simulation {
public:
    void getOutputMode(int argc, char *argv[], Options &options);
    void deployBattalions(Options &options);
    
private:
    vector<Planet> planets;
    vector<General> generals;
    uint32_t battles = 0;
};


#endif /* simulation_h */
