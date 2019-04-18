#ifndef __str_filter_h__
#define __str_filter_h__

#include <string>


class StrFilter {
public:
    std::string process(
            std::string &&data,
            const std::string &locale_name) const;
private:
    void _remove_punct(
            std::string &data) const;
    std::string _to_lower(
            std::string &&data,
            const std::string &locale_name) const;
    void _remove_new_lines(
            std::string &data) const;
};


#endif // __str_filter_h__
