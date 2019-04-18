#ifndef __string_view_h__
#define __string_view_h__

#include <algorithm>
#include <iterator>
#include <string>
#include <cctype>


class StringView {
public:
    class Itr {
    public:
        Itr(std::string &data, 
            std::string::value_type sep)
            : itr_{data.begin()}, 
              begin_{data.begin()}, 
              end_{data.end()}, 
              sep_{sep},
              not_separator_{sep}
        {
            itr_ = std::find_if(
                data.begin(), data.end(), 
                not_separator_
            );
        }

        Itr(std::string::iterator itr, 
            std::string::value_type sep)
            : itr_{itr}, begin_{itr}, 
                end_{itr}, sep_{sep} {}

        std::string operator * () {
            auto from = itr_;
            auto to = std::find_if_not(
                    itr_, end_, 
                    not_separator_
            );
            auto n = std::distance(from, to);

            std::string tmp;
            tmp.resize(n);
            std::copy(from, to, tmp.begin());

            return tmp;
        }

        Itr & operator ++ () {
            // pass symbols to the first separator
            itr_ = std::find_if_not(
                itr_, end_, 
                not_separator_
            );
            // pass all separators until first symbol
            itr_ = std::find_if(
                itr_, end_, 
                not_separator_
            );
            return *this;
        }

        Itr operator ++ (int) {
            Itr tmp{*this};
            this->operator ++ ();
            return tmp;
        }

        friend
        bool operator == (
                const StringView::Itr &lhs, 
                const StringView::Itr &rhs) 
        {
            return lhs.itr_ == rhs.itr_;
        }

        friend
        bool operator != (
                const StringView::Itr &lhs, 
                const StringView::Itr &rhs) 
        {
            return !(lhs == rhs);
        }
        
    private:
        struct IsNotSeparator {
            std::string::value_type sep;
            bool operator()(std::string::value_type c) const {
                return c != sep && !std::isspace(c);
            }
        };

        std::string::iterator itr_;
        std::string::iterator begin_;
        std::string::iterator end_;
        std::string::value_type sep_;
        IsNotSeparator not_separator_;
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

private:
    std::string data_;
};


#endif // __string_view_h__
