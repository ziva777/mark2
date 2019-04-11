#include "misc.h"

std::string
extract(
        std::string &&s, 
        std::string::value_type left,
        std::string::value_type right)
{
    size_t p1 = s.find(left);
    size_t p2 = s.rfind(right);

    if (p1 == std::string::npos || p2 == std::string::npos)
        return std::string();

    return s.substr(p1+1, p2-p1-1);
}

std::pair<
    std::string, std::string
> split_in_pos(
        std::string &&s,
        std::string::size_type pos)
{
    if (pos == std::string::npos)
        return std::make_pair(
            std::move(s), 
            std::move(std::string()));
    return std::make_pair(
        std::move(s.substr(0, pos)), 
        std::move(s.substr(pos + 1, s.size() - pos)));
}

std::pair<
    std::string, std::string
> split_left(
        std::string &&s,
        std::string::value_type sep)
{
    return split_in_pos(std::move(s), s.find(sep));
}

std::pair<
    std::string, std::string
> split_right(
        std::string &&s,
        std::string::value_type sep)
{
    return split_in_pos(std::move(s), s.rfind(sep));
}

std::list<
    std::string
> split_to_list(
        std::string &&s,
        std::string::value_type sep)
{
    std::list<std::string> res;
    std::string buff;

    for (auto c : s) {
        if (c == sep) {
            res.emplace_back(std::move(buff));
        } else {
            buff.push_back(std::move(c));
        }
    }

    res.emplace_back(std::move(buff));
    return res;
}
