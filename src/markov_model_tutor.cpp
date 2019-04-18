#include "markov_model_tutor.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <vector>

#include "token_iterator.h"

namespace msg_str {
    static const char ORDER_ERROR[] = "Corpus must be >= order!";
}

void 
MarkovModelTutor::train(
        MarkovModel &model,
        std::string &&data,
        bool calc_weights,
        char data_tokes_sep)
{
    // std::istringstream iss(data);
    // std::istream_iterator<std::string> begin{iss}, end{};
    // train_from_itr_(model, begin, end);

    train_from_itr_(
        model, 
        TokenIterator::begin(data, TOKENS_SEPARATOR), 
        TokenIterator::end(data, TOKENS_SEPARATOR)
    );

    if (calc_weights)
        model.calc_weights();
}

template<typename ITRERATOR>
void 
MarkovModelTutor::train_from_itr_(
        MarkovModel &model,
        ITRERATOR begin,
        ITRERATOR end)
{
    size_t n = model.order();
    bool corpus_empty = true;

    for (; begin != end; ++begin) {
        auto itr = begin;
        size_t i = 0;

        for (; i != n && itr != end; ++i, ++itr) {
        }

        if (itr != end) {
            corpus_empty = false;

            State state;
            for (auto itr2 = begin; itr2 != itr; ++itr2) {
                state.emplace_back(*itr2);
            }

            if (itr != end) {
                model.place(std::move(state), std::move(*itr));
            }
        }
    }

    if (corpus_empty) {
        throw std::logic_error(msg_str::ORDER_ERROR);
    }

    /*size_t order = model.order();
    State window;
    auto itr = begin;

    for (size_t i = 0; i != order &&  itr != end; ++i, ++itr) {
        window.emplace_back(*itr);
    }

    if (window.size() >= order) {
        for (;itr != end; ++itr) {
            model.place(window, *itr);
            window.pop_front();
            window.emplace_back(*itr);
        }        
    } else {
        throw std::logic_error(msg_str::ORDER_ERROR);
    }*/
}
