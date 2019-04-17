#ifndef __string_view_h__
#define __string_view_h__

#include <algorithm>
#include <string>


class StringView {
public:
    class Itr {
    public:
        Itr(std::string &data, 
            std::string::value_type sep)
            : itr_{data.begin()}, begin_{data.begin()}, 
                end_{data.end()}, sep_{sep} 
        {
            // TODO: move to operator *
            while (itr_ != end_ && *itr_ == sep_) {
                ++itr_;
            }
        }

        Itr(std::string::iterator itr, 
            std::string::value_type sep)
            : itr_{itr}, begin_{itr}, 
                end_{itr}, sep_{sep} {}

        std::string operator * () {
            // TODO: optimize
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
                if (*itr_ == sep_) {
                    break;
                }
            }

            for (;itr_ != end_; ++itr_) {
                if (*itr_ != sep_) {
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

private:
    std::string data_;
};


#endif // __string_view_h__
