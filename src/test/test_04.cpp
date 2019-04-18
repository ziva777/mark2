#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <list>

#include "../token_iterator.h"

void
foo()
{
#   define STR \
        "     Взявшись хлопотать об издании Повестей И. П. Белкина, предлагаемых ныне\n"\
        "публике, мы желали к оным присовокупить хотя краткое жизнеописание покойного"

    std::string s1, s2;
    {
        std::string s{STR};
        char sep = ' ';
        auto itr = TokenIterator::begin(s, sep);
        auto end = TokenIterator::end(s, sep);

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

int main()
{
    std::cout << "Test #04\n";
    foo();
    return EXIT_SUCCESS;
}
