#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <list>

#include "../string_view.h"

void
foo()
{
#   define STR \
        "     Взявшись хлопотать об издании Повестей И. П. Белкина, предлагаемых ныне\n"\
        "публике, мы желали к оным присовокупить хотя краткое жизнеописание покойного"

    std::string s1, s2;
    {
        StringView view(STR);
        char sep = ' ';
        auto itr = view.begin(sep);
        auto end = view.end(sep);

        while (itr != end) {
            s1 += ">" + *itr++ + "< ";
        }
    }

    {
        std::istringstream iss{STR};
        std::istream_iterator<std::string> itr{iss}, end{};

        while (itr != end) {
            s2 += ">" + *itr++ + "< ";
        }
    }

    assert(s1 == s2);
    std::cout << "Test #04.01 PASSED" << std::endl;
}

void 
bar()
{
    using State = std::list<std::string>;

    State state = {
        {"one"}, {"one"}
    };

    std::string s = std::accumulate(
        state.begin(), state.end(), std::string{},
        [](const std::string &a, const std::string &b){
            return a + " " + b;
        }
    );

    size_t h = std::accumulate(
        state.begin(), state.end(), size_t{0},
        [](size_t hash, const std::string &s){
            size_t h = std::hash<std::string>{}(s);
            hash ^= h + 0x9e3779b9 + (hash) + (hash << 6) + (h >> 2);
            return hash;
        }
    );

    
    assert(3388165740184751273ULL == h);
    std::cout << "Test #04.02 PASSED" << std::endl;
}

int main()
{
    std::cout << "Test #04\n";
    foo();
    bar();
    return EXIT_SUCCESS;
}
