
#include <bitset>
#include <vector>
#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/json_struct.h"
#include "../dictionary.h"

#ifndef MODULES_INTERACTIONS_H
#define MODULES_INTERACTIONS_H

namespace interactions
{

    struct StateMachine
    {
        int state_machine_id;
        int current_state;
        std::vector<int> states;
    };

    struct Encounter
    {
        int encounter_id;
        std::vector<interactions::StateMachine> state_machines;

    };


    /*
        Encounter:

        0) doesnt know about the wold
        1) knows about the wolf
        2) seen the wolf
        3a) killed the wolf
        3b) not killed the wolf
    */


    // (internal) Gets index of current state
    int _get_state_index(interactions::StateMachine &sm);


    // Encounter will have multiple state machines?



    extern std::bitset<1000> history;

    // initialize interaction bitmap
    void init();

    // Checks if given interaction happen
    bool did_it_happen(int interaction_id);

    // Record interaction by interaction_id
    int record(int interaction_id);

    // Clear interactions (make them all false)
    void clear();

}

#endif

