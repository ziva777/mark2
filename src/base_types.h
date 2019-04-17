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
    size_t operator()(const State &state) const 
    {
        std::string s = std::accumulate(
            state.begin(), state.end(), std::string(),
            [](
                const std::string &a,
                const std::string &b
            ) {
                return a + '_' + b;
            }
        );
        return std::hash<std::string>()(s);
    };
};


#endif // __base_types_h__
