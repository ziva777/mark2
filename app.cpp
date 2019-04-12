#include "app.h"

#include <fstream>
#include <cstdlib>

#include "misc.h"
#include "pipe_read.h"
#include "str_filter.h"
#include "markov_model.h"
#include "markov_model_tutor.h"
#include "markov_model_serializer.h"

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
    auto bash_prompt = [](size_t order){
        std::cout << "Enter n = " << order << " word(s). ";
        std::cout << "Or type \\q for exit.\n";
        std::cout << "phrase: ";
    };
    auto get_order = [](){
        size_t order = 0;
        std::string line;
        std::cin >> line;


        try {
            order = stoll(line);
        } catch (...) {
            order = std::string::npos;            
        }

        return order;
    };

    MarkovModel model = load_from_file_(arg);
    StrFilter filter;

    std::cout << "Model is ready!\n";

    bash_prompt(model.order());

    for (std::string line; std::getline(std::cin, line);) {
        if (!line.empty()) {
            if (line == "\\q") {
                return EXIT_SUCCESS;
            }

            trim(line);
            filter.process(line, LOCALE);
            auto tokens = split_to_list(std::move(line), ' ');

            State state;
            size_t n_count = 0;

            for (auto itr = tokens.begin(); itr != tokens.end(); ++itr) {
                if (!itr->empty()) {
                    state.emplace_back(*itr);
                    ++n_count;
                }
            }

            if (n_count < model.order()) {
                std::cout << "Warning: you are enter less tokens than";
                std::cout << "model order. Enter ";
                std::cout << model.order() << " word(s).\n";
                break;
            } else if (n_count != model.order()) {
                std::cout << "Warning: you are enter more tokens than ";
                std::cout << "model order, extra tokes are thrown away.\n";
                // TODO: remove extra tokens
                state.erase(
                    ++(++state.begin())
                );
            } 

            {
                std::cout << "order : ";
                size_t count = get_order();

                if (count != std::string::npos) {
                    std::string res = model.generate(std::move(state), count);
                    std::cout << ">" << res << "\n";
                }
            }

            std::cout << "phrase: ";
        } 
    }

    return EXIT_SUCCESS;
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

MarkovModel 
App::load_from_file_(
        const std::string &file)
{
    std::ifstream ifs;
    std::stringstream ss;

    ifs.open(file);

    for (std::string l; std::getline(ifs, l);) {
        ss << l << '\n';
    }

    MarkovModelSerializer serializer;
    MarkovModel model = serializer.from_stream(ss);
    return model;
}
