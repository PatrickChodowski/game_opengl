
#include <bitset>
#include <vector>
#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/json_struct.h"
#include "../dictionary.h"

#ifndef MODULES_INTERACTIONS_H
#define MODULES_INTERACTIONS_H

#define HISTORY_EVENT_0 0

namespace interactions
{
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

