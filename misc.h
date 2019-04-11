#ifndef __misc_h__
#define __misc_h__

#include <utility>
#include <string>
#include <list>


struct StringJoin {
    std::string spacer;
    std::string operator()(
            const std::string &a,
            const std::string &b) const
    {
        if (a.empty())
            return b;
        return a + "," + b;
    }
};


std::string
extract(
        std::string &&s, 
        std::string::value_type left,
        std::string::value_type right);

std::pair<
    std::string, std::string
> split_in_pos(
        std::string &&s,
        std::string::size_type pos);

std::pair<
    std::string, std::string
> split_left(
        std::string &&s,
        std::string::value_type sep);

std::pair<
    std::string, std::string
> split_right(
        std::string &&s,
        std::string::value_type sep);

std::list<
    std::string
> split_to_list(
        std::string &&s,
        std::string::value_type sep);

#endif // __misc_h__
