#include <unordered_map>
#include <iostream>
#include <iterator>
#include <cstddef>
#include <cstdlib>
#include <ostream>
#include <numeric>
#include <sstream>
#include <random>
#include <string>
#include <tuple>
#include <list>

#include "misc.h"

#include "str_filter.h"
#include "markov_model.h"
#include "markov_model_tutor.h"



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

                std::cout << "STATES:\n";
                for (auto item : state) {
                    std::cout << item << " ";
                }
                std::cout << "\n";

                std::cout << "TRANSITIONS:\n";
                Transitions t = std::get<0>(tr);
                size_t w = std::get<1>(tr);

                for (auto &item : t) {
                    std::cout << item.first << " : ";
                    std::cout << item.second << " ";
                }
                std::cout << "\n";

                if (ok) {
                    model.place(std::move(state), std::move(tr));
                } else {
                    std::cerr << "Error - can't parse line #";
                    std::cerr << count << "\n";
                }
            }

            return model;
        } else {
            throw; // Can't parse the model order!
        }
    } else {
        throw; // Input stream are empty!
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


std::string data1 = 
    "One fish two fish red fish blue fish"
    "";
std::string data2 = 
    "One meat two meat red meat blue meat"
    "";
std::string data3 = 
    "One veg two veg red veg blue veg blue meat red fish"
    "";
std::string data_src1 = 
    "order:1\n"
    "[veg]   : [{blue:2, red:1, two:1}, 4]\n"
    "[fish]  : [{blue:1, red:1, two:1}, 3]\n"
    "[one]   : [{veg:1, meat:1, fish:1}, 3]\n"
    "[red]   : [{veg:1, meat:1, fish:2}, 4]\n"
    "[two]   : [{veg:1, meat:1, fish:1}, 3]\n"
    "[blue]  : [{veg:1, meat:2, fish:1}, 4]\n"
    "[meat]  : [{blue:1, red:2, two:1}, 4]\n"
    "";
std::string data_src2 = 
    "order:2\n"
    "[fish,blue]     : [{fish:1}, 1]\n"
    "[red,fish]  : [{blue:1}, 1]\n"
    "[fish,red]  : [{fish:1}, 1]\n"
    "[one,fish]  : [{two:1}, 1]\n"
    "[two,fish]  : [{red:1}, 1]\n"
    "[fish,two]  : [{fish:1}, 1]\n"
    "";



int
main()
{

    // std::cout << extract("", '[', ']') << std::endl;
    // std::cout << extract(" ", '[', ']') << std::endl;
    // std::cout << extract(" [veg]", '[', ']') << std::endl;
    // std::cout << extract(" [veg", '[', ']') << std::endl;
    // std::cout << extract(" veg]", '[', ']') << std::endl;
    // std::cout << extract("  [{blue:2, red:1, two:1}, 4] ", '[', ']') << std::endl;

    // auto p1 = split_left("{1}, {2}, {3}", ',');
    // std::cout << p1.first << ":" << p1.second << std::endl;
    // auto p2 = split_right("{1}, {2}, {3}", ',');
    // std::cout << p2.first << ":" << p2.second << std::endl;

    // std::list<std::string> s1 = split_to_list("1,2,3,4,popa", ',');
    // for (const std::string &s : s1) {
    //     std::cout << s << "|";
    // }
    // std::cout << std::endl;

    const char locale[] = "ru_RU.UTF-8";
    const size_t model_order = 2;

    std::string res;

    StrFilter filter;
    filter.process(data1, locale);
    filter.process(data2, locale);
    filter.process(data3, locale);

    // MarkovModel model(model_order);
    // MarkovModelTutor tutor;
    MarkovModelSerializer serializer;

    std::istringstream os1(data_src1);
    MarkovModel model1 = serializer.from_stream(os1);
    serializer.to_stream(model1, std::cout);

    // std::istringstream os2(data_src2);
    // MarkovModel model2 = serializer.from_stream(os2);
    // serializer.to_stream(model2, std::cout);

    // std::cout << "Take #1\n";
    // tutor.train(model, std::move(data1), true);
    // serializer.to_stream(model, std::cout);

    // res = model.generate(State{{"fish"}}, 0);
    // res = model.generate(State{{"one"}, {"fish"}}, 10);
    // std::cout << "RES1 : " << res << "\n";

    // std::cout << "Take #2\n";
    // tutor.train(model, std::move(data2), true);
    // serializer.to_stream(model, std::cout);
    // res = model.generate(State{{"one"}}, 10);
    // // res = model.generate(State{{"one"}, {"fish"}}, 10);
    // std::cout << "RES2 : " << res << "\n";

    // std::cout << "Take #3\n";
    // tutor.train(model, std::move(data3), true);
    // serializer.to_stream(model, std::cout);
    // res = model.generate(State{{"one"}}, 10);
    // // res = model.generate(State{{"one"}, {"fish"}}, 10);
    // std::cout << "RES3 : " << res << "\n";

    return EXIT_SUCCESS;
}





// #include <unordered_map>
// #include <iostream>
// #include <cstddef>
// #include <cstdlib>
// #include <random>
// #include <string>
// #include <list>

// using Atom = std::string;
// using Transition = std::unordered_map<Atom, size_t>;
// using Basket = std::list<Atom>;

// size_t
// sum_transitions_weights(const Transition &tr)
// {
//     size_t sum = 0;

//     for (auto &item : tr) {
//         sum += item.second;
//     }

//     return sum;
// }

// std::string
// random_str(const Transition &tr, size_t v)
// {
//     for (auto &item : tr) {
//         if (v < item.second)
//             return item.first;
//         v -= item.second;
//     }
//     return tr.begin()->first;
// }

// int
// main()
// {
//     Transition tr = {
//         {"apple", 70},
//         {"mango", 30},
//         {"banan", 10},
//     };

//     std::random_device dev;
//     std::mt19937 gen(dev());
//     std::string s;

//     Basket apple, mango, banan;

//     for (int i = 0; i != 1000; ++i) {
//         std::uniform_int_distribution<size_t> distr(0, sum_transitions_weights(tr));
//         s = random_str(tr, distr(gen));

//         if (s == "apple")
//             apple.emplace_back(std::move(s));
//         else if (s == "mango")
//             mango.emplace_back(std::move(s));
//         else if (s == "banan")
//             banan.emplace_back(std::move(s));
//     }

//     std::cout << "apple basket : " << apple.size() << "\n";
//     std::cout << "mango basket : " << mango.size() << "\n";
//     std::cout << "banan basket : " << banan.size() << "\n";
//     // size_t n = distr(gen);

//     return EXIT_SUCCESS;
// }