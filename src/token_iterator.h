#ifndef __string_view_h__
#define __string_view_h__

#include <algorithm>
#include <iterator>
#include <string>
#include <cctype>


class TokenIterator {
public:
    TokenIterator(
            std::string &data, 
            std::string::value_type sep)
        : TokenIterator_{data.begin()}, 
          begin_{data.begin()}, 
          end_{data.end()}, 
          sep_{sep},
          not_separator_{sep}
    {
        TokenIterator_ = std::find_if(
            data.begin(), data.end(), 
            not_separator_
        );
    }

    TokenIterator(
            std::string::iterator TokenIterator, 
            std::string::value_type sep)
        : TokenIterator_{TokenIterator}, begin_{TokenIterator}, 
            end_{TokenIterator}, sep_{sep} {}

    std::string operator * () {
        auto from = TokenIterator_;
        auto to = std::find_if_not(
                TokenIterator_, end_, 
                not_separator_
        );
        auto n = std::distance(from, to);

        std::string tmp;
        tmp.resize(n);
        std::copy(from, to, tmp.begin());

        return tmp;
    }

    TokenIterator & operator ++ () {
        // pass symbols to the first separator
        TokenIterator_ = std::find_if_not(
            TokenIterator_, end_, 
            not_separator_
        );
        // pass all separators until first symbol
        TokenIterator_ = std::find_if(
            TokenIterator_, end_, 
            not_separator_
        );
        return *this;
    }

    TokenIterator operator ++ (int) {
        TokenIterator tmp{*this};
        this->operator ++ ();
        return tmp;
    }

    friend
    bool operator == (
            const TokenIterator &lhs, 
            const TokenIterator &rhs) 
    {
        return lhs.TokenIterator_ == rhs.TokenIterator_;
    }

    friend
    bool operator != (
            const TokenIterator &lhs, 
            const TokenIterator &rhs) 
    {
        return !(lhs == rhs);
    }

    static TokenIterator begin(
            std::string &data, 
            char sep) 
    {
        return TokenIterator{data, sep};
    }

    static TokenIterator end(
            std::string &data, 
            char sep) 
    {
        return TokenIterator{data.end(), sep};
    }
    
private:
    struct IsNotSeparator {
        std::string::value_type sep;
        bool operator()(std::string::value_type c) const {
            return c != sep && !std::isspace(c);
        }
    };

    std::string::iterator TokenIterator_;
    std::string::iterator begin_;
    std::string::iterator end_;
    std::string::value_type sep_;
    IsNotSeparator not_separator_;
};


#endif // __string_view_h__
