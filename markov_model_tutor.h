#ifndef __markov_model_tutor_h__
#define __markov_model_tutor_h__

#include <iterator>
#include <string>

#include "markov_model.h"


class MarkovModelTutor {
public:
    void train(
            MarkovModel &model,
            std::string &&data,
            bool calc_weights = false);
private:
    using TokensItr = std::istream_iterator<std::string>;

    void train_from_itr_(
            MarkovModel &model,
            TokensItr begin,
            TokensItr end);
};


#endif // __markov_model_tutor_h__
