#ifndef __str_filter_h__
#define __str_filter_h__

#include <string>


class StrFilter {
public:
    void process(
            std::string &data,
            std::string locale_name) const;
private:
    void _remove_punct(
            std::string &data) const;
    void _to_lower(
            std::string &data,
            std::string locale_name) const;
};


#endif // __str_filter_h__
