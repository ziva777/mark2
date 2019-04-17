#include "markov_model_tutor.h"

#include <stdexcept>
#include <sstream>

namespace msg_str {
    static const char ORDER_ERROR[] = "Corpus must be >= order!";
}

void 
MarkovModelTutor::train(
        MarkovModel &model,
        std::string &&data,
        bool calc_weights)
{
    std::istringstream iss(data);
    TokensItr begin{iss}, end{};

    train_from_itr_(model, begin, end);

    if (calc_weights)
        model.calc_weights();
}

void 
MarkovModelTutor::train_from_itr_(
        MarkovModel &model,
        TokensItr begin,
        TokensItr end)
{
    State window;
    size_t order = model.order();
    TokensItr itr = begin;

    for (size_t i = 0; i != order &&  itr != end; ++i, ++itr) {
        window.emplace_back(*itr);
    }

    if (window.size() >= order) {
        while (itr != end) {
            model.place(window, *itr);
            window.pop_front();
            window.emplace_back(*itr);
            ++itr;
        }        
    } else {
        throw std::logic_error(msg_str::ORDER_ERROR);
    }
}
