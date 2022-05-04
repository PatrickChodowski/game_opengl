
#include "interactions.h"
#include <iostream>
#include <bitset>
#include <vector>

#include "../dependencies/parallel_hashmap/phmap.h"
#include "../dependencies/json_struct.h"
#include "../dictionary.h"

namespace interactions
{
    std::bitset<1000> history;

    void init()
    {
        for(int i = 0; i<interactions::history.size(); i++)
        {
            interactions::history[i] = false;
        }
    }

    int record(int interaction_id)
    {
        interactions::history[interaction_id] = true;
        return interaction_id;
    }

    bool did_it_happen(int interaction_id)
    {
        // Interaction ID is index of bool in interactions::history
        return interactions::history[interaction_id];
    }

    void clear()
    {
        for(int i = 0; i<interactions::history.size(); i++)
        {
            interactions::history[i] = false;
        }
    }

}
