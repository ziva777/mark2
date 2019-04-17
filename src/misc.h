#ifndef __misc_h__
#define __misc_h__

#include <algorithm>
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
        return a + spacer + b;
    }
};


/* [qwer] -> qwer */
std::string
extract(
        std::string &&s, 
        std::string::value_type left,
        std::string::value_type right);

/* 123:456 -> (123,456) */
std::pair<
    std::string, std::string
> split_in_pos(
        std::string &&s,
        std::string::size_type pos);

/* 123:456:789 -> (123,456:789) */
std::pair<
    std::string, std::string
> split_left(
        std::string &&s,
        std::string::value_type sep);

/* 123:456:789 -> (123:456,789) */
std::pair<
    std::string, std::string
> split_right(
        std::string &&s,
        std::string::value_type sep);

/* 1,2,3 -> [1,2,3] */
std::list<
    std::string
> split_to_list(
        std::string &&s,
        std::string::value_type sep);

/* " trim" -> "trim" */
inline void
ltrim(std::string &s)
{
    s.erase(
        s.begin(),
        std::find_if(
            s.begin(), s.end(),
            [](std::string::value_type c) { 
                return !std::isspace(c); 
            }
        )
    );
}

/* "trim " -> "trim" */
inline void
rtrim(std::string &s)
{
    s.erase(
        std::find_if(
            s.rbegin(), s.rend(),
            [](std::string::value_type c) { 
                return !std::isspace(c); 
            }
        ).base(), 
        s.end()
    );
}

/* " trim " -> "trim" */
inline void
trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

#endif // __misc_h__
