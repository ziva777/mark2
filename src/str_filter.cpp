#include "str_filter.h"

#include <algorithm>
#include <codecvt>
#include <locale>

void 
StrFilter::process(
    std::string &data,
    std::string locale_name) const
{
    _remove_punct(data);
    _to_lower(data, locale_name);
    _remove_new_lines(data);
}

void 
StrFilter::_remove_punct(
    std::string &data) const
{
    std::replace_if(
            data.begin(), data.end(),
            [] (int c) {
                return std::ispunct(c);
            }, 
            ' ');
}

void 
StrFilter::_to_lower(
    std::string &data,
    std::string locale_name) const
{
    std::locale utf8_loc(locale_name);
    std::wstring_convert<
        std::codecvt_utf8<wchar_t>
    > conv;
    std::wstring ws = conv.from_bytes(std::move(data));

    std::transform(
        ws.begin(), ws.end(),
        ws.begin(),
        [&](wchar_t c) {
            return (wchar_t)std::tolower(c, utf8_loc);
        }
    );

    data = conv.to_bytes(ws);
}

void 
StrFilter::_remove_new_lines(
        std::string &data) const
{
    std::replace_if(
        data.begin(), data.end(),
        [](std::string::value_type c) {
            return c == '\n' || c == '\r' || c == '\t';
        },
        ' '
    );
}
