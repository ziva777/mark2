#ifndef __base_types_h__
#define __base_types_h__

#include <unordered_map>
#include <numeric>
#include <cstddef>
#include <string>
#include <tuple>
#include <list>

struct StateHash;

constexpr int TRANSITIONS_ID = 0;
constexpr int WEIGHST_ID = 1;

using Atom = std::string;
using State = std::list<Atom>;
using Transitions = std::unordered_map<
    Atom,
    size_t
>;
using TransitionsWithWeights = std::tuple<
    Transitions, 
    size_t // sum of transitions weights
>;
using StateMachine = std::unordered_map<
    State,
    TransitionsWithWeights,
    StateHash
>;


struct StateHash {
    size_t operator()(
            const State &state) const 
    {
        return std::accumulate(
            state.begin(), state.end(), size_t{0},
            [](size_t hash, const std::string &s){
                size_t h = std::hash<std::string>{}(s);
                hash ^= h + 0x9e3779b9 + (hash) + (hash << 6) + (h >> 2);
                return hash;
            }
        );
    };
};


#endif // __base_types_h__
