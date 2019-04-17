#include "markov_model.h"

#include <iostream>
#include <sstream>
#include <random>

#include "misc.h"

namespace msg_str {
    static const char GENERATE_ERROR[] = 
        "Origin state not eq to order of model!";
}

auto random_transition_choice = [](
    const Transitions &transitions, 
    size_t random_weight
) {
    for (auto &transition : transitions) {
        if (random_weight < transition.second)
            return transition.first;
        random_weight -= transition.second;
    }
    return transitions.begin()->first;
};

template<typename RANDOM_ENGINE>
std::string transition_choice(
        const Transitions &transitions,
        size_t sum_weights,
        RANDOM_ENGINE &random_generator)
{
    std::uniform_int_distribution<size_t> 
        distribution(0, sum_weights);
    return
        random_transition_choice(
            transitions, distribution(random_generator));
}


MarkovModel::MarkovModel(
        size_t order)
: order_(order)
{
    
}

void 
MarkovModel::calc_weights()
{
    auto sum_transitions_weights = [](
        const Transitions &transitions
    ) {
        size_t sum = 0;

        for (auto &transition : transitions) {
            sum += transition.second;
        }

        return sum;
    };

    for (auto &item : machine_) {
        auto &transitions = item.second;
        std::get<WEIGHST_ID>(transitions) = 
            sum_transitions_weights(std::get<TRANSITIONS_ID>(transitions));
    }
}

void 
MarkovModel::place(
        const State &state, 
        const Atom &atom)
{
    // core[state][atom] += 1;

    auto res = 
        machine_.emplace(
            state, 
            TransitionsWithWeights{
                Transitions{
                    {atom, 1}
                }, 
                0
            }
        );

    auto itr = res.first;
    bool created = res.second;

    if (!created) {
        Transitions &tr = std::get<TRANSITIONS_ID>(itr->second);
        auto res = tr.emplace(atom, 0);
        auto item = res.first;
        ++item->second;
    }
}

void 
MarkovModel::place(
        State &&state, 
        TransitionsWithWeights &&transitions)
{
    machine_.emplace(
        state, 
        transitions
    );
}

std::string 
MarkovModel::generate(
        State &&origin,
        size_t n) const
{
    if (origin.size() != order_) {
        throw std::logic_error(msg_str::GENERATE_ERROR);
    }

    std::stringstream ss;

    for (auto &item : origin) {
        ss << item << " ";
    }

    bool stop = false;
    std::random_device random_device;
    std::mt19937_64 random_generator(random_device());

    while (n-- && !stop) {
        try {
            auto &transitions = machine_.at(origin);

            if (!std::get<TRANSITIONS_ID>(transitions).empty()) {
                std::string s = 
                    transition_choice(
                        std::get<TRANSITIONS_ID>(transitions),
                        std::get<WEIGHST_ID>(transitions), 
                        random_generator);
                ss << s << " ";
                origin.push_back(s);
                origin.pop_front();
            } else {
                stop = true;
            }
        } catch (const std::out_of_range &) {
            stop = true;
        }
    }

    return ss.str();
}
