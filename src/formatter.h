#ifndef __formatter_h__
#define __formatter_h__


struct Formatter {
    static const char list_begin = '[';
    static const char list_end = ']';
    static const char dict_begin = '{';
    static const char dict_end = '}';
    static const char item_sep = ',';
    static const char dict_sep = ',';
    static const char dict_kv_sep = ':';
    static const char state_transition_sep = ':';
    static const char order_sep = ':';
    static const char order_key[];
};


#endif // __formatter_h__
