#ifndef __app_h__
#define __app_h__

#include <iostream>
#include <string>
#include <list>

#include "markov_model.h"


class App {
public:
    using Args = std::list<std::string>;

    App(Args &&args);
    int exec();

private:
    static const char MODE_CREATE_MODEL[];
    static const char MODE_EXEC_MODEL[];
    static const char LOCALE[];

    using Urls = std::list<std::string>;

    Args args_;

    int create_mode_(
            const std::string &arg);
    int exec_mode_(
            const std::string &arg);
    int unknown_mode_();

    void invalid_args_msg_();

    MarkovModel create_model_from_urls_(
            size_t order, 
            Urls &&urls);
    MarkovModel load_from_file_(
            const std::string &file);
};


#endif // __app_h__
