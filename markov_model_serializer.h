#ifndef __markov_model_serializer_h__
#define __markov_model_serializer_h__

#include <sstream>
#include <string>

#include "markov_model.h"


class MarkovModelSerializer {
public:
    void to_stream(
            const MarkovModel &model, 
            std::ostream &stream);
    MarkovModel from_stream(
            std::istream &stream);
    
private:
    std::pair<
        size_t, bool
    > parse_order_(
            std::string &&s) const;

    std::tuple<
        State, TransitionsWithWeights, bool
    > parse_model_(
            std::string &&s) const;

    std::pair<
        State, bool
    > parse_model_state_(
        std::string &&s) const;

    std::pair<
        TransitionsWithWeights, bool
    > parse_model_transitions_(
        std::string &&s) const;
};


#endif // __markov_model_serializer_h__
