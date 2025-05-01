//
//  simulation.cpp
//  p2-a
//
//  Created by Joyce Zhou on 2/15/23.
//AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include <getopt.h>
#include <vector>
#include <queue>
#include <iostream>
#include <sstream>
#include "simulation.h"
#include "P2random.h"
using namespace std;

Battalion::Battalion() {
    general_id =  0;
    planet_id  = 0;
    force_sense = 0;
    num_troops = 0;
    priority_id = 0;
}

AmbushNotebook::AmbushNotebook() {
    state = State::Zero;
}

AttackNotebook::AttackNotebook() {
    state = State::Zero;
}

Planet::Planet() {}

void Planet::jediInput(Battalion &b, Options &options,
                       vector<General> &generals, uint32_t &battles) {
    
    bool sith_attack = false;
    
    jedi.push(b);

    if (!sith.empty() && sith.top().force_sense >= jedi.top().force_sense) {
        sith_attack = true;
    }

    while (sith_attack) {
        if (options.verbose) {
            cout << "General " << sith.top().general_id << "'s battalion attacked General "
            << jedi.top().general_id << "'s battalion on planet " << jedi.top().planet_id << ". ";
        }
        
        uint32_t deaths = sithAttack(options, generals);
        battles++;
        
        if (options.verbose) {
            cout << deaths << " troops were lost.\n";
        }
        
        if (sith.empty() || jedi.empty() || sith.top().force_sense < jedi.top().force_sense) {
            sith_attack = false;
        }
    }

}

void Planet::sithInput(Battalion &b, Options &options, vector<General> &generals, uint32_t &battles) {
    
    bool sith_attack = false;
    
    sith.push(b);
    
    if (!jedi.empty() && sith.top().force_sense >= jedi.top().force_sense) {
        sith_attack = true;
    }
    
    while (sith_attack) {
        if (options.verbose) {
            cout << "General " << sith.top().general_id << "'s battalion attacked General "
            << jedi.top().general_id << "'s battalion on planet " << jedi.top().planet_id << ". ";
        }
        
        uint32_t deaths = sithAttack(options, generals);
        battles++;
        
        if (options.verbose) {
            cout << deaths << " troops were lost.\n";
        }
        if (sith.empty() || jedi.empty() || sith.top().force_sense < jedi.top().force_sense) {
            sith_attack = false;
        }
    }
}

uint32_t Planet::calculateMedian() {
    uint32_t median = 0;
    if (maxPQ.empty() && minPQ.empty()) {
        median = 0;
    }
    else if (maxPQ.size() == minPQ.size()) {
        median = (maxPQ.top() + minPQ.top())/2;
    }
    else if (maxPQ.size() > minPQ.size()) {
        median = maxPQ.top();
    }
    else {
        median = minPQ.top();
    }
    return median;
}

void Planet::jediUpdateNotebook(uint32_t force_sense, uint32_t timestamp) {
    if (attack.state == State::Zero) {
        attack.state = State::One;
        attack.weakest_jedi = force_sense;
        attack.jedi_timestamp = timestamp;
    }
    else if (attack.state == State::One && force_sense < attack.weakest_jedi) {
        attack.weakest_jedi = force_sense;
        attack.jedi_timestamp = timestamp;
    }
    else if (attack.state == State::Two) {
        if (attack.maybe_best_jedi == 0 || force_sense < attack.maybe_best_jedi) {
            attack.maybe_best_jedi = force_sense;
            attack.maybe_jedi_timestamp = timestamp;
        }
    }//if ..attack
    
    if (ambush.state == State::One && force_sense <= ambush.strongest_sith) {
        ambush.state = State::Two;
        ambush.weakest_jedi = force_sense;
        ambush.jedi_timestamp = timestamp;
        ambush.greatest_difference = ambush.strongest_sith - ambush.weakest_jedi;
    }
    else if (ambush.state == State::Two) {
        if (ambush.maybe_best_sith == 0 && force_sense < ambush.weakest_jedi) {
            ambush.weakest_jedi = force_sense;
            ambush.jedi_timestamp = timestamp;
            ambush.greatest_difference = ambush.strongest_sith - ambush.weakest_jedi;
        }
        else if (ambush.maybe_best_sith != 0) {
            uint32_t maybe_test1 = 0, maybe_test2 = 0;
            if (force_sense < ambush.maybe_best_sith) {
                maybe_test1 = ambush.maybe_best_sith - force_sense;
            }
            if (force_sense < ambush.strongest_sith) {
                maybe_test2 = ambush.strongest_sith - force_sense;
            }
            
            if (maybe_test2 >= maybe_test1) {
                if (ambush.greatest_difference < maybe_test2) {
                    ambush.weakest_jedi = force_sense;
                    ambush.jedi_timestamp = timestamp;
                    ambush.greatest_difference = maybe_test2;
                }
            }
            else {
                if (ambush.greatest_difference < maybe_test1) {
                    ambush.strongest_sith = ambush.maybe_best_sith;
                    ambush.sith_timestamp = ambush.maybe_sith_timestamp;
                    ambush.weakest_jedi = force_sense;
                    ambush.jedi_timestamp = timestamp;
                    ambush.greatest_difference = maybe_test1;
                }
            }
        }
    }//if ..ambush
}

void Planet::sithUpdateNotebook(uint32_t force_sense, uint32_t timestamp) {
    if (attack.state == State::One && force_sense >= attack.weakest_jedi) {
        attack.state = State::Two;
        attack.strongest_sith = force_sense;
        attack.sith_timestamp = timestamp;
        attack.greatest_difference = attack.strongest_sith - attack.weakest_jedi;
    }
    else if (attack.state == State::Two) {
        if (attack.maybe_best_jedi == 0  && force_sense > attack.strongest_sith) {
            attack.strongest_sith = force_sense;
            attack.sith_timestamp = timestamp;
            attack.greatest_difference = attack.strongest_sith - attack.weakest_jedi;
        }
        else if (attack.maybe_best_jedi != 0) {
            uint32_t maybe_test1 = 0, maybe_test2 = 0;
            if (force_sense > attack.maybe_best_jedi) {
                maybe_test1 = force_sense - attack.maybe_best_jedi;
            }
            if (force_sense > attack.weakest_jedi) {
                maybe_test2 = force_sense - attack.weakest_jedi;
            }
            
            if (maybe_test2 >= maybe_test1) {
                if (attack.greatest_difference < maybe_test2) {
                    attack.strongest_sith = force_sense;
                    attack.sith_timestamp = timestamp;
                    attack.greatest_difference = maybe_test2;
                }
            }
            else {
                if (attack.greatest_difference < maybe_test1) {
                    attack.weakest_jedi = attack.maybe_best_jedi;
                    attack.jedi_timestamp = attack.maybe_jedi_timestamp;
                    attack.strongest_sith = force_sense;
                    attack.sith_timestamp = timestamp;
                    attack.greatest_difference = maybe_test1;
                }
            }
        }
    }//if ..attack
    
    if (ambush.state == State::Zero) {
        ambush.state = State::One;
        ambush.strongest_sith = force_sense;
        ambush.sith_timestamp = timestamp;
    }
    else if (ambush.state == State::One && force_sense > ambush.strongest_sith) {
        ambush.strongest_sith = force_sense;
        ambush.sith_timestamp = timestamp;
    }
    else if (ambush.state == State::Two) {
        if (ambush.maybe_best_sith == 0 || force_sense > ambush.maybe_best_sith) {
            ambush.maybe_best_sith = force_sense;
            ambush.maybe_sith_timestamp = timestamp;
        }
    }//if ..ambush
}

void Planet::printAmbushAttack(uint32_t planet_id) {
    if (ambush.state == State::Zero || ambush.state == State::One) {
        cout << "A movie watcher would not see an interesting ambush on planet " << planet_id << ".\n";
    }
    else {
        cout << "A movie watcher would enjoy an ambush on planet " << planet_id << " with Sith at time "
            << ambush.sith_timestamp << " and Jedi at time " << ambush.jedi_timestamp
            << " with a force difference of " << ambush.greatest_difference << ".\n";
    }
    if(attack.state == State::Zero || attack.state == State::One) {
        cout << "A movie watcher would not see an interesting attack on planet " << planet_id << ".\n";
    }
    else {
        cout << "A movie watcher would enjoy an attack on planet " << planet_id << " with Jedi at time "
            << attack.jedi_timestamp << " and Sith at time " << attack.sith_timestamp
            << " with a force difference of " << attack.greatest_difference << ".\n";
    }
}

void Simulation::getOutputMode(int argc, char *argv[], Options &options) {
    int index = 0, choice = 0;
    opterr = false;
    option longOpts[] = {{ "verbose", no_argument, nullptr, 'v' },
                            { "median", no_argument, nullptr, 'm' },
                            { "general-eval", no_argument, nullptr, 'g' },
                            { "watcher", no_argument, nullptr, 'w' },
                            { nullptr, 0, nullptr, '\0' }};
    
    while ((choice = getopt_long(argc, argv, "vmgw", longOpts, &index)) != -1) {
        switch (choice) {
            case 'v':
                options.verbose = true;
                break;
            case 'm':
                options.median =  true;
                break;
            case 'g':
                options.general = true;
                break;
            case 'w':
                options.watcher = true;
                break;
            default:
                cerr << "Unknown command line option\n";
                exit(1);
        }
    }
}

void Simulation::deployBattalions(Options &options) {
    stringstream ss;
    string junk, inputMode;
    uint32_t num_gen, num_planets;
    getline(cin, junk);
    cin >> junk >> inputMode >> junk >> num_gen >> junk >> num_planets;
    
    
    for (uint32_t i = 0; i < num_planets; ++i) {
        Planet p;
        planets.push_back(p);
    }//for ..planets
    
    for (uint32_t j = 0; j < num_gen; ++j) {
        General g;
        generals.push_back(g);
    }//for ..generals
    
    if (inputMode == "PR") {
        uint32_t seed, num_deploys, rate;
        cin >> junk >> seed >> junk >> num_deploys >> junk >> rate;
        P2random::PR_init(ss, seed, num_gen, num_planets, num_deploys, rate);
    }
    
    istream &inputStream = inputMode == "PR" ? ss : cin;
    
    uint32_t timestamp, general_id, planet_id, force_sense, num_troops, priority_id = 0, current_time = 0;
    string side;
    char c;
    
    while (inputStream >> timestamp >> side >> c >> general_id >> c >> planet_id >> c >> force_sense >> c >> num_troops) {
        if (general_id >= num_gen) {
            cerr << "Invalid general ID\n";
            exit(1);
        }
        if (planet_id >= num_planets) {
            cerr << "Invalid planet ID\n";
            exit(1);
        }
        if (timestamp < current_time) {
            cerr << "Invalid decreasing timestamp\n";
            exit(1);
        }
        if (force_sense <= 0) {
            cerr << "Invalid force sensitivity level\n";
            exit(1);
        }
        if (num_troops <= 0) {
            cerr << "Invalid number of troops\n";
            exit(1);
        }//error checking
        
        if (options.median == true && current_time != timestamp) {
            if (battles > 0) {
                for (uint32_t k = 0; k < num_planets; ++k) {
                    if (planets[k].calculateMedian() != 0) {
                        cout << "Median troops lost on planet " << k << " at time " << current_time << " is "
                        << planets[k].calculateMedian() << ".\n";
                    }
                } //for ..print median
            }//if ..battles
        }//if ..median
        current_time = timestamp;
        
        Battalion b(general_id, planet_id, force_sense, num_troops, priority_id);
        
        
        if (side == "JEDI") {
            if (options.general == true) {
                generals[general_id].num_jedi += b.num_troops;
            }
            planets[planet_id].jediUpdateNotebook(b.force_sense, timestamp);
            planets[planet_id].jediInput(b, options, generals, battles);
        }
        else {
            if (options.general == true) {
                generals[general_id].num_sith += b.num_troops;
            }
            planets[planet_id].sithUpdateNotebook(b.force_sense, timestamp);
            planets[planet_id].sithInput(b, options, generals, battles);
        }//if ..side
        
        priority_id++;
    }//while ..inputStream
    
    if (options.median == true && battles > 0) {
        for (uint32_t k = 0; k < num_planets; ++k) {
            if (planets[k].calculateMedian() != 0) {
                cout << "Median troops lost on planet " << k << " at time " << current_time << " is "
                << planets[k].calculateMedian() << ".\n";
            }
        } //for ..print median
    }//if ..print median
    
    cout << "---End of Day---\n";
    cout << "Battles: " << battles << "\n";
    
    if (options.general == true) {
        cout << "---General Evaluation---\n";
        for (uint32_t i = 0; i < generals.size(); ++i) {
            uint32_t total = generals[i].num_jedi + generals[i].num_sith;
            uint32_t num_survived = total - generals[i].num_losses;
            cout << "General " << i << " deployed " << generals[i].num_jedi
            << " Jedi troops and " << generals[i].num_sith << " Sith troops, and "
            << num_survived << "/" << total << " troops survived.\n";
        }
    }
    if (options.watcher == true) {
        cout << "---Movie Watcher---\n";
        for (uint32_t j = 0; j < planets.size(); ++j) {
            planets[j].printAmbushAttack(j);
        }
    }
    
}
