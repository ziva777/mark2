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

class StringView {
public:
    // using List = std::list<std::string>;

    class Itr {
    public:
        Itr(std::string &data, 
            std::string::value_type sep)
            : itr_{data.begin()}, begin_{data.begin()}, 
                end_{data.end()}, sep_{sep} 
        {
            while (itr_ != end_ && *itr_ == sep_) {
                ++itr_;
            }
        }

        Itr(std::string::iterator itr, 
            std::string::value_type sep)
            : itr_{itr}, begin_{itr}, 
                end_{itr}, sep_{sep} {}

        std::string operator * () {
            std::string tmp;

            

            for (auto itr = itr_; itr != end_; ++itr) {
                if (*itr != sep_) {
                    tmp.push_back(*itr);
                } else {
                    break;
                }

            }
            return tmp;
        }

        Itr & operator ++ () {
            for (;itr_ != end_; ++itr_) {
                if ((*itr_ == sep_) || (*itr_ == '\n')) {
                    break;
                }
            }

            for (;itr_ != end_; ++itr_) {
                if ((*itr_ != sep_) && (*itr_ != '\n')) {
                    break;
                }
            }

            return *this;
        }
        Itr operator ++ (int) {
            Itr tmp{*this};
            this->operator ++ ();
            return tmp;
        }

        friend
        bool operator == (const StringView::Itr &lhs, const StringView::Itr &rhs) {
            return lhs.itr_ == rhs.itr_;
        }
        friend
        bool operator != (const StringView::Itr &lhs, const StringView::Itr &rhs) {
            return !(lhs == rhs);
        }
        
    private:
        std::string::iterator itr_;
        std::string::iterator begin_;
        std::string::iterator end_;
        std::string::value_type sep_;
    };

    StringView() = delete;
    StringView(std::string &&data)
        : data_{std::move(data)} {}


    Itr begin(char sep) {
        return Itr{data_, sep};
    }

    Itr end(char sep) {
        return Itr{data_.end(), sep};
    }

    // void split(List &list, char sep) const {
    //     std::string tmp;

    //     for (auto c : data_) {
    //         if (c != sep)
    //             tmp.push_back(c);
    //         else {
    //             if (!tmp.empty())
    //                 list.emplace_back(std::move(tmp));
    //         }
    //     }

    //     if (!tmp.empty()) {
    //         list.emplace_back(std::move(tmp));
    //     }
    // }

private:
    std::string data_;
};



void
foo()
{
#   define STR \
        "     Взявшись хлопотать об издании Повестей И. П. Белкина, предлагаемых ныне\n"\
        "публике, мы желали к оным присовокупить хотя краткое жизнеописание покойного"
    {
        StringView view(STR);
        char sep = ' ';
        StringView::Itr itr = view.begin(sep);
        StringView::Itr end = view.end(sep);

        while (itr != end) {
            std::cout << ">" << *itr++ << "< ";
        }

        std::cout << std::endl;
    }

    {
        std::istringstream iss{STR};
        std::istream_iterator<std::string> itr{iss}, end{};

        while (itr != end) {
            std::cout << ">" << *itr++ << "< ";
        }

        std::cout << std::endl;
    }
}

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
    foo();
    return EXIT_SUCCESS;
}
