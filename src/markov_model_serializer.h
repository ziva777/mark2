#ifndef __markov_model_serializer_h__
#define __markov_model_serializer_h__

#include <sstream>
#include <string>

#include "formatter.h"
#include "markov_model.h"


class MarkovModelSerializer {
public:
    void to_stream(
            const MarkovModel &model, 
            std::ostream &stream);
    MarkovModel from_stream(
            std::istream &stream);
    
private:
    template<typename FORMATTER = Formatter>
    struct FormatWrapper : FORMATTER {};
    static FormatWrapper<> formatter_;

    void serialize_order_(
            const MarkovModel &model, 
            std::ostream &stream);
    void serialize_model_(
            const MarkovModel &model, 
            std::ostream &stream);
    std::string serialize_model_state_(
            const State &state);
    std::string serialize_model_transitions_(
            const TransitionsWithWeights &wt);

    std::pair<
        size_t, bool
    > parse_order_(
            std::string &&s) const;

    std::pair<
        std::pair<State, TransitionsWithWeights>, 
        bool
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

    std::pair<
        Transitions, bool
    > parse_transitions_(
            std::string &&s) const;

    std::pair<
        size_t, bool
    > parse_weight_(
            std::string &&s) const;
};


#endif // __markov_model_serializer_h__
