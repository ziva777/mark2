#ifndef __markov_model_h__
#define __markov_model_h__

#include <cstddef>
#include <string>

#include "base_types.h"


class MarkovModel {
public:
    MarkovModel(size_t order);

    void calc_weights();
    void place(
            const State &state, 
            const Atom &atom);
    void place(
            State &&state, 
            TransitionsWithWeights &&transitions);

    std::string generate(
            State &&origin,
            size_t n) const;

    size_t order() const { return order_; }

    // StateMachineItr begin() { return machine_.begin(); };
    // StateMachineItr end() { return machine_.end(); };

    // StateMachineCItr cbegin() const { return machine_.begin(); };
    // StateMachineCItr cend() const { return machine_.end(); };

    StateMachine::iterator begin() { return machine_.begin(); }
    StateMachine::iterator end() { return machine_.end(); }
    StateMachine::const_iterator begin() const { return machine_.begin(); }
    StateMachine::const_iterator end() const { return machine_.end(); }

private:
    size_t order_;
    StateMachine machine_;
    StateMachine::iterator itr;
};


#endif // __markov_model_h__
