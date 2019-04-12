#include <iostream>
#include <cstdlib>
#include <string>

#include "misc.h"
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


#include <vector>
#include <list>

#include "pipe_read.h"











//////////////////////////////////////

int
main()
{
    std::list<std::string> files = {
        {"file1"},
        {"file2"},
    };

    for (int i = 0; i != 100; ++i) {
        PipeRead pipe;
        std::string cmd =
            "curl -s file:///home/marina/programming/git/mark2/case_003.txt";
        auto res = pipe.read(cmd);
        // std::cout << "out = " << res.first << std::endl;
        std::cout << "ok  = " << res.second << std::endl;
    }

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

    return EXIT_SUCCESS;
}
