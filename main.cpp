#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>

#include "misc.h"
#include "pipe_read.h"
#include "str_filter.h"
#include "markov_model.h"
#include "markov_model_tutor.h"
#include "markov_model_serializer.h"


std::string data1 = 
    "One fish two fish red fish blue fish"
    "";
std::string data2 = 
    "One meat two meat red meat blue meat"
    "";
std::string data3 = 
    "One veg two veg red veg blue veg blue meat red fish"
    "";
std::string data_src1 = 
    "order:1\n"
    "[veg]   : [{blue:2, red:1, two:1}, 4]\n"
    "[fish]  : [{blue:1, red:1, two:1}, 3]\n"
    "[one]   : [{veg:1, meat:1, fish:1}, 3]\n"
    "[red]   : [{veg:1, meat:1, fish:2}, 4]\n"
    "[two]   : [{veg:1, meat:1, fish:1}, 3]\n"
    "[blue]  : [{veg:1, meat:2, fish:1}, 4]\n"
    "[meat]  : [{blue:1, red:2, two:1}, 4]\n"
    "";
std::string data_src2 = 
    "order:2\n"
    "[fish,blue]     : [{fish:1}, 1]\n"
    "[red,fish]  : [{blue:1}, 1]\n"
    "[fish,red]  : [{fish:1}, 1]\n"
    "[one,fish]  : [{two:1}, 1]\n"
    "[two,fish]  : [{red:1}, 1]\n"
    "[fish,two]  : [{fish:1}, 1]\n"
    "";

void
mode1(size_t order)
{
    /* Create model and serialize it into cout */
}

void
mode2()
{
    /* Load model from file and execute generation */
}

/*
mode1 - ./mark2 c <order>
mode2 - ./mark2 e <input_model.txt> 
*/

void 
mode_unknown()
{

}

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

    int create_mode_(const std::string &arg);
    int exec_mode_(const std::string &arg);
    int unknown_mode_();

    void invalid_args_msg_();

    MarkovModel create_model_from_urls_(
            size_t order, 
            Urls &&urls);
};

const char App::MODE_CREATE_MODEL[] = "c";
const char App::MODE_EXEC_MODEL[] = "e";
const char App::LOCALE[] = "ru_RU.UTF-8";

App::App(
        Args &&args)
: args_{std::move(args)}
{
}

int
App::exec()
{
    if (args_.size() != 2) {
        invalid_args_msg_();
        return EXIT_FAILURE;
    }

    auto itr = args_.begin();
    const auto &arg1 = *itr++;
    const auto &arg2 = *itr++;
    int result_code;

    if (arg1 == MODE_CREATE_MODEL) {
        result_code = create_mode_(arg2);
    } else if (arg1 == MODE_EXEC_MODEL) {
        result_code = exec_mode_(arg2);
    } else {
        result_code = unknown_mode_();
    }

    return result_code;
}

int 
App::create_mode_(const std::string &arg)
{
    try {
        size_t order = std::stoll(arg);
        Urls urls;

        for (std::string url; std::getline(std::cin, url);) {
            if (!url.empty())
                urls.emplace_back(std::move(url));
        }

        if (urls.empty()) {
            std::cout << "OOPS! The URLs list are empty!" << std::endl;
            return EXIT_FAILURE;
        }
        
        MarkovModel model = create_model_from_urls_(order, std::move(urls));
        MarkovModelSerializer serializer;

        serializer.to_stream(model, std::cout);

        return EXIT_SUCCESS;

    } catch (const std::invalid_argument &e) {
        std::cerr << "OOPS! " << e.what() << std::endl;
    } catch (const std::out_of_range &e) {
        std::cerr << "OOPS! " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "OOPS! unknown error"<< std::endl;
    }

    return EXIT_FAILURE;
}

int 
App::exec_mode_(const std::string &arg)
{

}

int 
App::unknown_mode_()
{
    invalid_args_msg_();
    return EXIT_FAILURE;
}

void 
App::invalid_args_msg_()
{
    std::cout << "Invalid args!\n";
    std::cout << "To build model use: ./mark2 " 
              << MODE_CREATE_MODEL << " <order>\n";
    std::cout << "To run model use  : ./mark2 "
              << MODE_EXEC_MODEL << " <model_file>";
    std::cout << std::endl;
}

MarkovModel 
App::create_model_from_urls_(
        size_t order, 
        Urls &&urls)
{
    MarkovModel model(order);
    MarkovModelTutor tutor;
    StrFilter filter;
    PipeRead pipe;

    for (auto &url: urls) {
        std::string cmd = "curl -s " + url;
        auto res = pipe.read(cmd);

        if (res.second) {
            if (!res.first.empty()) {
                filter.process(res.first, LOCALE);
                tutor.train(model, std::move(res.first), false);
            }
        } else {
            std::cerr << "OOPS! Can't process file ";
            std::cerr << url << std::endl;
        }
    }

    model.calc_weights();

    return model;
}


int
main(int argc, char *argv[])
{
    std::list<std::string> args;

    for (int i = 1; i != argc; ++i) {
        args.emplace_back(argv[i]);
    }

    App app(std::move(args));
    return app.exec();

    // if (argc == 3) {
    //     App app(
    //         std::list<std::string>{
    //             {argv[1]}, {argv[2]}
    //         }
    //     );

    //     return app.exec();
    //     // std::string mode{argv[1]};
    //     // std::string arg{argv[2]};

    //     // switch (mode) {
    //     //     case "c":
    //     //         mode1();
    //     //         break;
    //     //     case "e":
    //     //         mode2();
    //     //         break;
    //     //     default:
    //     //         mode_unknown();
    //     //         return EXIT_FAILED;
    //     //         break;
    //     // }
    // } else {
    //     std::cout << "Invalid args!\n";
    //     std::cout << "To build model use: ./mark2 c <order>\n";
    //     std::cout << "To run model use: ./mark2 e <model_file>";
    //     std::cout << std::endl;
    //     return EXIT_FAILURE;
    // }
    // if (argc == 2) {
    //     try {
    //         size_t order = std::stoll(argv[1]);
    //         mode1(order);
    //     } catch (const std::invalid_argument &e) {
    //         std::cerr << "OOPS! Order must be number" << e.what() << std::endl;
    //     } catch (const std::out_of_range &e) {
    //         std::cerr << "OOPS! Order out of range " << e.what() << std::endl;
    //     } catch (...) {
    //         std::cerr << "OOPS! unknown"<< std::endl;
    //     }
    // } else if (argc == 3) {
    //     try {
    //         size_t order = std::stoll(argv[1]);
    //         mode1(order);
    //     } catch (const std::invalid_argument &e) {
    //         std::cerr << "OOPS! Order must be number" << e.what() << std::endl;
    //     } catch (const std::out_of_range &e) {
    //         std::cerr << "OOPS! Order out of range " << e.what() << std::endl;
    //     } catch (...) {
    //         std::cerr << "OOPS! unknown"<< std::endl;
    //     }
    // } else {
    //     std::cout << "Invalid args!\n";
    //     std::cout << "To build model use: ./mark2 <order>\n";
    //     std::cout << "To run model use: ./mark2 <order> <model_file>";
    //     std::cout << std::endl;
    // }
    // std::list<std::string> files = {
    //     {"file1"},
    //     {"file2"},
    // };

    // for (int i = 0; i != 100; ++i) {
    //     PipeRead pipe;
    //     std::string cmd =
    //         "curl -s file:///home/marina/programming/git/mark2/case_003.txt";
    //     auto res = pipe.read(cmd);
    //     // std::cout << "out = " << res.first << std::endl;
    //     std::cout << "ok  = " << res.second << std::endl;
    // }

    // std::vector<int> v;
    // size_t n = 1024*1024*200; // 10 Mb
    // v.resize(n);

    // for (size_t i = 0; i != n; ++i) {
    //     v[i] = i;
    // }

    // FILE *pipe = popen("echo \"123\" && sleep 30", "r");
    // if (pipe) {
    //     constexpr int buff_size = 64;
    //     char buff[buff_size];
    //     char *s;

    //     while ((s = fgets(buff, buff_size, pipe)) != nullptr) {
    //         std::cout << buff << std::endl;
    //     }
    //     // std::cout << "DONE\n";
    //     pclose(pipe);
    // } else {
    //     std::cout << "ERROR\n";
    // }

    // std::string s;
    // std::cin >> s;

    // v.clear();
    // v.shrink_to_fit();

    // std::cin >> s;

    // std::cout << extract("", '[', ']') << std::endl;
    // std::cout << extract(" ", '[', ']') << std::endl;
    // std::cout << extract(" [veg]", '[', ']') << std::endl;
    // std::cout << extract(" [veg", '[', ']') << std::endl;
    // std::cout << extract(" veg]", '[', ']') << std::endl;
    // std::cout << extract("  [{blue:2, red:1, two:1}, 4] ", '[', ']') << std::endl;

    // auto p1 = split_left("{1}, {2}, {3}", ',');
    // std::cout << p1.first << ":" << p1.second << std::endl;
    // auto p2 = split_right("{1}, {2}, {3}", ',');
    // std::cout << p2.first << ":" << p2.second << std::endl;

    // std::list<std::string> s1 = split_to_list("1,2,3,4,popa", ',');
    // for (const std::string &s : s1) {
    //     std::cout << s << "|";
    // }
    // std::cout << std::endl;

    // const char locale[] = "ru_RU.UTF-8";
    // const size_t model_order = 2;

    // std::string res;

    // StrFilter filter;
    // filter.process(data1, locale);
    // filter.process(data2, locale);
    // filter.process(data3, locale);

    // // MarkovModel model(model_order);
    // // MarkovModelTutor tutor;
    // MarkovModelSerializer serializer;

    // std::istringstream os1(data_src1);
    // MarkovModel model1 = serializer.from_stream(os1);
    // serializer.to_stream(model1, std::cout);

    // std::istringstream os2(data_src2);
    // MarkovModel model2 = serializer.from_stream(os2);
    // serializer.to_stream(model2, std::cout);

    // std::cout << "Take #1\n";
    // tutor.train(model, std::move(data1), true);
    // serializer.to_stream(model, std::cout);

    // res = model.generate(State{{"fish"}}, 0);
    // res = model.generate(State{{"one"}, {"fish"}}, 10);
    // std::cout << "RES1 : " << res << "\n";

    // std::cout << "Take #2\n";
    // tutor.train(model, std::move(data2), true);
    // serializer.to_stream(model, std::cout);
    // res = model.generate(State{{"one"}}, 10);
    // // res = model.generate(State{{"one"}, {"fish"}}, 10);
    // std::cout << "RES2 : " << res << "\n";

    // std::cout << "Take #3\n";
    // tutor.train(model, std::move(data3), true);
    // serializer.to_stream(model, std::cout);
    // res = model.generate(State{{"one"}}, 10);
    // // res = model.generate(State{{"one"}, {"fish"}}, 10);
    // std::cout << "RES3 : " << res << "\n";

    // return EXIT_SUCCESS;
}
