#ifndef __markov_model_tutor_h__
#define __markov_model_tutor_h__

#include <string>
#include <list>

#include "markov_model.h"


class MarkovModelTutor {
public:
    void train(
            MarkovModel &model,
            std::string &&data,
            bool calc_weights = false,
            char data_tokes_sep = ' ');
    
private:
    static const char TOKENS_SEPARATOR = ' ';
    template<typename ITRERATOR>
    void train_from_itr_(
            MarkovModel &model,
            ITRERATOR begin,
            ITRERATOR end);
};


#endif // __markov_model_tutor_h__
