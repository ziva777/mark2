#include "markov_model_serializer.h"

#include <stdexcept>
#include <iostream>
#include <tuple>

#include "misc.h"

void 
MarkovModelSerializer::to_stream(
        const MarkovModel &model, 
        std::ostream &stream)
{
    StringJoin str_sum{","};
    stream << "order:" << std::to_string(model.order()) << "\n";

    for (auto itr = model.begin(); itr != model.end(); ++itr) {
        auto &item = *itr;
        auto &state = item.first;
        auto &transitions = item.second;

        std::string k = std::accumulate(
            state.begin(), state.end(), std::string(),
            str_sum
        );

        std::string v;
        bool comma = false;

        for (auto &transition : std::get<TRANSITIONS_ID>(transitions)) {
            if (comma)
                v += ", ";

            v += transition.first;
            v += ":";
            v += std::to_string(transition.second);

            comma = true;
        }

        stream << "[" << k << "] \t: [{" << v << "}";
        stream << ", " 
               << std::to_string(std::get<WEIGHST_ID>(transitions)) 
               << "]\n";
    }
}

MarkovModel 
MarkovModelSerializer::from_stream(
        std::istream &stream)
{    
    std::string s;

    if (std::getline(stream, s)) {
        auto res = parse_order_(std::move(s));
        bool ok = res.second;
        size_t order = res.first;
        MarkovModel model(order);

        if (ok) {
            size_t count = 0;

            while (std::getline(stream, s)) {
                auto res = parse_model_(std::move(s));
                State &state = std::get<0>(res);
                TransitionsWithWeights &tr = std::get<1>(res);
                bool ok = std::get<2>(res);

                ++count;

                // std::cout << "STATES:\n";
                // for (auto item : state) {
                //     std::cout << item << " ";
                // }
                // std::cout << "\n";

                // std::cout << "TRANSITIONS:\n";
                Transitions t = std::get<0>(tr);
                size_t w = std::get<1>(tr);

                // for (auto &item : t) {
                //     std::cout << item.first << " : ";
                //     std::cout << item.second << " ";
                // }
                // std::cout << "\n";

                if (ok) {
                    model.place(std::move(state), std::move(tr));
                } else {
                    std::cerr << "Error - can't parse line #";
                    std::cerr << count << "\n";
                }
            }

            return model;
        } else {
            throw std::logic_error("Can't parse the model order!");
        }
    } else {
        throw std::logic_error("Input stream are empty!");
    }

    return MarkovModel{0};
}

std::pair<
    size_t, bool
> MarkovModelSerializer::parse_order_(
        std::string &&s) const
{
    size_t order = 0;
    bool ok = false;

    if (!s.empty()) {
        auto res = split_left(std::move(s), ':');

        if (res.first == "order") {
            order = std::stoll(res.second);
            ok = true;
        }
    }

    return std::make_pair(order, ok);
}

std::pair<
    State, bool
> MarkovModelSerializer::parse_model_state_(
        std::string &&s) const
{
    State st{};
    bool ok = false;

    s = extract(std::move(s), '[', ']');
    auto l = split_to_list(std::move(s), ',');

    for (auto &item : l) {
        st.push_back(std::move(item));
        ok = true;
    }


    return std::make_pair(std::move(st), ok);
}

std::pair<
    TransitionsWithWeights, bool
> MarkovModelSerializer::parse_model_transitions_(
    std::string &&s) const
{
    auto parse_transitions = [](
        std::string &&s
    ) {
        Transitions ret{};
        auto l = split_to_list(std::move(s), ',');

        for (auto item : l) {
            auto res = split_left(std::move(item), ':');
            auto &k = res.first;
            auto v = std::stoll(res.second);
            ret.emplace(k, v);
            // tmp[k] = v;
        }

        bool ok = true;
        return std::make_pair(ret, ok);
    };

    auto parse_weight = [](
        std::string &&s
    ) {
        bool ok = true;
        size_t w = std::stoll(s);
        return std::make_pair(w, ok);
    };

    bool ok = false;

    s = extract(std::move(s), '[', ']');
    auto l = split_right(std::move(s), ',');
    auto &s_t = l.first;
    auto &s_w = l.second;

    s_t = extract(std::move(s_t), '{', '}');

    Transitions tr;
    size_t tr_w;
    bool tr_ok, tr_w_ok;
    std::tie(tr, tr_ok) = parse_transitions(std::move(s_t));
    std::tie(tr_w, tr_w_ok) = parse_weight(std::move(s_w));
    ok = tr_ok && tr_w_ok;

    return std::make_pair(std::move(TransitionsWithWeights{tr, tr_w}), ok);
}

std::tuple<
    State, TransitionsWithWeights, bool
> MarkovModelSerializer::parse_model_(
        std::string &&s) const
{
    bool ok = false;
    std::string s_st, s_tr;

    std::tie(s_st, s_tr) =
        split_left(std::move(s), ':');

    auto res_st = parse_model_state_(std::move(s_st));
    auto res_tr = parse_model_transitions_(std::move(s_tr));
    auto &st = res_st.first;
    auto &tr = res_tr.first;

    ok = res_st.second && res_tr.second;

    return std::make_tuple(std::move(st), std::move(tr), std::move(ok));
}
