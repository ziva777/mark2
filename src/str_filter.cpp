#include "str_filter.h"

#include <algorithm>
#include <codecvt>
#include <locale>

namespace msg_str {
    static const char LOCALE_UNSUPPORTED[] = 
        "Your local is not supported - ";
}


struct ToLower {
    std::locale loc;
    std::wstring::value_type operator()(
        std::wstring::value_type c) const 
    {
        return std::tolower(c, loc);
    }
};


std::string 
StrFilter::process(
    std::string &&data,
    const std::string &locale_name) const
{
    _remove_punct(data);
    data = _to_lower(std::move(data), locale_name);
    _remove_new_lines(data);
    return data;
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

std::string 
StrFilter::_to_lower(
    std::string &&data,
    const std::string &locale_name) const
{
    try {
        std::locale utf8_loc(locale_name);
        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring ws = conv.from_bytes(std::move(data));

        std::transform(
            ws.begin(), ws.end(),
            ws.begin(),
            ToLower{utf8_loc}
        );

        data = conv.to_bytes(ws);
    } catch (const std::runtime_error &) {
        std::string msg = msg_str::LOCALE_UNSUPPORTED;
        msg += locale_name;
        throw std::logic_error(msg);
    }

    return data;
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
