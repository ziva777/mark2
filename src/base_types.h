#ifndef __base_types_h__
#define __base_types_h__

#include <unordered_map>
#include <numeric>
#include <cstddef>
#include <string>
#include <tuple>
#include <list>

using Atom = std::string;
using State = std::list<Atom>;
using Transitions = std::unordered_map<
    Atom,
    size_t
>;

class TransitionsWithWeights {
public:
    TransitionsWithWeights(Transitions &&transitions, size_t weights_sum)
        : transitions_{std::move(transitions)}, weights_sum_{weights_sum} {}

    Transitions & transitions() { return transitions_; }
    const Transitions & transitions() const { return transitions_; }
    size_t weights_sum() const { return weights_sum_; }

    void calc_weights_sum() {
        weights_sum_ = std::accumulate(
            transitions_.begin(), transitions_.end(), size_t{0},
            [](size_t s, auto &transition){
                return s + transition.second;
            }
        );
    }

private:
    Transitions transitions_{};
    size_t weights_sum_{0};
};


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


using StateMachine = std::unordered_map<
    State,
    TransitionsWithWeights,
    StateHash
>;

#endif // __base_types_h__
