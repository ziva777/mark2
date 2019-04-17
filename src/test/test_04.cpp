#include <functional>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cassert>
#include <cstdlib>
#include <string>
#include <list>

int main()
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

    // std::cout << " str = " << s << std::endl;
    // std::cout << "hash = " << h << std::endl;

    assert(3388165740184751273ULL == h);

    return EXIT_SUCCESS;
}
