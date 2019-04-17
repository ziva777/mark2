#include "markov_model_tutor.h"

#include <stdexcept>
// #include <algorithm>
// #include <iterator>
#include <sstream>
#include <vector>

namespace msg_str {
    static const char ORDER_ERROR[] = "Corpus must be >= order!";
}

class StringViewWindow {
public:
    StringViewWindow(std::string &&data, char sep, size_t size)
        : str_view_{std::move(data)}, sep_{sep}, size_{size},
            begin_{str_view_.begin(sep_)},
            end_{str_view_.end(sep_)}
    {
        // window_.reserve(size_);

        for (size_t i = 0; i != size_; ++i) {
            auto itr = begin_;

            for (size_t j = 0; j != i; ++j)
                ++itr;

            window_.push_back(itr);
        }
    }

    size_t size() const { return size_; }

    // bool is_valid()
    // {
    //     for (auto &item : window_) {
    //         if (item == end_) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }

    // bool next() 
    // {
    //     for (auto &item : window_) {
    //         if (++item == end_) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }

    void fill(MarkovModel &model)
    {

        auto next = [](
            std::vector<StringView::Itr> &window, 
            StringView::Itr end
        ) {
            for (auto &item : window) {
                if (++item == end) {
                    return false;
                }
            }
            return true;
        };

        do {
            State state;

            for (size_t i = 0; i != size_; ++i) {
                state.emplace_back(*window_[i]);
            }

            model.place(std::move(state), *window_[window_.size() - 1]);
        } while (next(window_, end_));
        // while (is_valid()) {
        //     State state;

        //     for (size_t i = 0; i != size_; ++i) {
        //         state.emplace_back(*window_[i]);
        //     }

        //     model.place(std::move(state), *window_[size_ - 1]);
        //     next();
        // }
    }

private:
    StringView str_view_;
    char sep_;
    size_t size_;
    std::vector<StringView::Itr> window_;
    StringView::Itr begin_;
    StringView::Itr end_;
};

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

    // {
    //     size_t n = model.order();
    //     StringViewWindow sw{std::move(data), TOKENS_SEPARATOR, n+1};
    //     sw.fill(model);
    // }

    {
        auto next = [](
            std::list<StringView::Itr> &window, 
            StringView::Itr end
        ) {
            for (auto &item : window) {
                if (++item == end) {
                    return false;
                }
            }
            return true;
        };

        StringView view{std::move(data)};
        std::list<StringView::Itr> window;
        // window.resize(model.order()+1);

        auto end = view.end(TOKENS_SEPARATOR);
        auto begin = view.begin(TOKENS_SEPARATOR);
        auto n = model.order();

        for (size_t i = 0; i != n+1; ++i) {
            auto itr = begin;

            for (size_t j = 0; j != i; ++j)
                ++itr;

            window.emplace_back(itr);
        }

        if (window.size() < n)
            throw std::logic_error(msg_str::ORDER_ERROR);

        do {
            State state;
            auto w_itr = window.begin();

            for (size_t i = 0; i != n; ++i) {
                state.emplace_back(*(*w_itr));
                ++w_itr;
            }

            model.place(std::move(state), *(*w_itr));
        } while (next(window, end));
    }

    // {
    //     StringView view{std::move(data)};
    //     std::vector<StringView::Itr> window;
    //     window.reserve(model.order()+1);

    //     auto end = view.end(TOKENS_SEPARATOR);
    //     auto begin = view.begin(TOKENS_SEPARATOR);

    //     for (size_t i = 0; i != model.order()+1; ++i) {
    //         auto itr = begin;

    //         for (size_t j = 0; j != i; ++j)
    //             ++itr;

    //         window.emplace_back(itr);
    //     }

    //     if (window.size() < model.order())
    //         throw std::logic_error(msg_str::ORDER_ERROR);

    //     auto next = [](
    //         std::vector<StringView::Itr> &window, 
    //         StringView::Itr end
    //     ) {
    //         for (auto &item : window) {
    //             if (++item == end) {
    //                 return false;
    //             }
    //         }
    //         return true;
    //     };

    //     do {
    //         State state;

    //         for (size_t i = 0; i != model.order(); ++i) {
    //             state.emplace_back(*window[i]);
    //         }

    //         model.place(std::move(state), *window[window.size() - 1]);
    //     } while (next(window, end));
    // }

    // StringView view{std::move(data)};

    // train_from_itr_(
    //     model, 
    //     view.begin(TOKENS_SEPARATOR), 
    //     view.end(TOKENS_SEPARATOR)
    // );

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
    size_t order = model.order();
    State window(order);
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
    }
}
