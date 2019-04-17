#include "markov_model_serializer.h"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <numeric>
#include <tuple>

#include "misc.h"

namespace msg_str {
    static const char LINE_PARSE_ERROR[] = "Error - can't parse line #";
    static const char ORDER_PARSE_ERROR[] = "Can't parse the model order!";
    static const char NO_DATA_ERROR[] = "Input stream are empty!";
}

void 
MarkovModelSerializer::to_stream(
        const MarkovModel &model, 
        std::ostream &stream)
{
    serialize_order_(model, stream);
    serialize_model_(model, stream);
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

        if (ok) {
            MarkovModel model(order);

            for (size_t count = 0; std::getline(stream, s); ++count) {
                auto res = parse_model_(std::move(s));
                bool ok = res.second;
                
                if (ok) {
                    auto &state = res.first.first;
                    auto &tr = res.first.second;
                    model.place(std::move(state), std::move(tr));
                } else {
                    std::cerr 
                        << msg_str::LINE_PARSE_ERROR << count << "\n";
                }
            }

            return model;
        } else {
            throw std::logic_error(msg_str::ORDER_PARSE_ERROR);
        }
    } else {
        throw std::logic_error(msg_str::NO_DATA_ERROR);
    }

    return MarkovModel{0};
}

void 
MarkovModelSerializer::serialize_order_(
    const MarkovModel &model, 
    std::ostream &stream)
{
    stream << formatter_.order_key
           << formatter_.order_sep
           << std::to_string(model.order()) 
           << "\n";
}

void 
MarkovModelSerializer::serialize_model_(
    const MarkovModel &model, 
    std::ostream &stream)
{
    for (auto itr = model.begin(); itr != model.end(); ++itr) {
        auto &state = itr->first;
        auto &transitions = itr->second;

        std::string k = serialize_model_state_(state);
        std::string v = serialize_model_transitions_(transitions);

        stream << formatter_.list_begin << k << formatter_.list_end;
        stream << " " << formatter_.state_transition_sep << " ";
        stream << formatter_.list_begin << v << formatter_.list_end;
        stream << "\n";
    }
}

std::string 
MarkovModelSerializer::serialize_model_state_(
    const State &state)
{
    return std::accumulate(
        state.begin(), state.end(), std::string(),
        StringJoin{std::string(1, formatter_.item_sep)}
    );
}

std::string 
MarkovModelSerializer::serialize_model_transitions_(
    const TransitionsWithWeights &wt)
{
    std::string v;
    bool comma = false;

    for (auto &transition : std::get<TRANSITIONS_ID>(wt)) {
        if (comma)
            v += formatter_.dict_sep;

        v += transition.first;
        v += formatter_.dict_kv_sep;
        v += std::to_string(transition.second);

        comma = true;
    }

    std::stringstream ss;
    ss << formatter_.dict_begin << v << formatter_.dict_end;
    ss << formatter_.item_sep;
    ss << std::to_string(std::get<WEIGHST_ID>(wt));
    return ss.str();
}

std::pair<
    size_t, bool
> MarkovModelSerializer::parse_order_(
        std::string &&s) const
{
    size_t order = 0;
    bool ok = false;

    if (!s.empty()) {
        auto res = split_left(std::move(s), formatter_.order_sep);

        if (res.first == formatter_.order_key) {
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

    auto l = split_to_list(
        std::move(
            extract(
                std::move(s), 
                formatter_.list_begin, 
                formatter_.list_end)
        ), 
        formatter_.item_sep
    );

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
    bool ok = false;
    auto l = split_right(
        std::move(
            extract(
                std::move(s), 
                formatter_.list_begin, formatter_.list_end
            )
        ), 
        formatter_.item_sep
    );
    auto &s_t = l.first;
    auto &s_w = l.second;

    s_t = extract(
        std::move(s_t), 
        formatter_.dict_begin, formatter_.dict_end
    );

    Transitions tr;
    size_t tr_w;
    bool tr_ok, tr_w_ok;
    std::tie(tr, tr_ok) = parse_transitions_(std::move(s_t));
    std::tie(tr_w, tr_w_ok) = parse_weight_(std::move(s_w));
    ok = tr_ok && tr_w_ok;

    return std::make_pair(
        std::move(
            TransitionsWithWeights{
                std::move(tr), tr_w
            }
        ), 
        ok
    );
}

std::pair<
    std::pair<State, TransitionsWithWeights>, 
    bool
> MarkovModelSerializer::parse_model_(
        std::string &&s) const
{
    bool ok = false;
    std::string s_st, s_tr;

    std::tie(s_st, s_tr) =
        split_left(std::move(s), formatter_.state_transition_sep);

    auto res_st = parse_model_state_(std::move(s_st));
    auto res_tr = parse_model_transitions_(std::move(s_tr));
    auto &st = res_st.first;
    auto &tr = res_tr.first;

    ok = res_st.second && res_tr.second;

    return std::make_pair(
            std::make_pair(std::move(st), std::move(tr)), 
            std::move(ok));
}

std::pair<
    Transitions, bool
> MarkovModelSerializer::parse_transitions_(
        std::string &&s) const
{
    bool ok = true;
    Transitions ret{};
    auto l = split_to_list(std::move(s), formatter_.dict_sep);

    for (auto item : l) {
        auto res = split_left(std::move(item), formatter_.dict_kv_sep);
        auto &k = res.first;
        auto v = std::stoll(res.second);
        ret.emplace(k, v);
        // tmp[k] = v;
    }

    return std::make_pair(ret, ok);
}

std::pair<
    size_t, bool
> MarkovModelSerializer::parse_weight_(
        std::string &&s) const
{
    bool ok = true;
    size_t w = 0;

    try {
        w = std::stoll(s);
    } catch (...) {
        ok = false;
    }

    return std::make_pair(w, ok); 
}
