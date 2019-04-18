#include "app.h"

#include <fstream>
#include <cstdlib>

#include "misc.h"
#include "pipe_read.h"
#include "markov_model.h"
#include "markov_model_tutor.h"
#include "markov_model_serializer.h"

const char App::MODE_CREATE_MODEL[] = "c";
const char App::MODE_EXEC_MODEL[] = "e";
const char App::CURL_CMD[] = "curl -s ";

namespace msg_str {
    static const char OOPS[] = "OOPS! ";
    static const char URLS_EMPTY[] = "The URLs list are empty!";
    static const char FILE_PROC_ERROR[] = "Can't process file ";
    static const char ARGS_ERROR[] = "Invalid args!";
    static const char UNKNOWN_ERROR[] = "unknown error";
    static const char MODEL_IS_READY[] = "The model is ready!";
    static const char N_COUNT_TO_GEN[] = "length: ";
    static const char PHRASE_TO_GEN[] = "phrase: ";
    static const char GEN_RESULT_PROMPT[] = "=>";
    static const char ENTER_ORDER_BEGIN[] = "Enter n = ";
    static const char ENTER_ORDER_CONT[] = " word(s). Or type ";
    static const char ENTER_ORDER_END[] = " for exit.";
    static const char BUILD_PROMPT[] = "To build model use: ./mark2 ";
    static const char RUN_PROMPT[] = "To run model use  : ./mark2 ";
    static const char BUILD_PROMPT_END[] = " <order>";
    static const char RUN_PROMPT_END[] = " <model_file>";

    static const char WARN_LESS_TOKENS_BEGIN[] = 
        "Warning: you are enter less tokens than "
        "model order. Enter ";
    static const char WARN_LESS_TOKENS_END[] = 
        " word(s).";
    static const char WARN_MORE_TOKENS_BEGIN[] = 
        "Warning: you are enter more tokens than "
        "model order, extra tokes are thrown away.";
    static const char WARN_MORE_TOKENS_END[] = 
        "";

    static const char EXIT_SEQ[] = "\\q";
}


App::App(
        Args &&args)
: args_{std::move(args)}
{
    locale_ = std::getenv("LANG");
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
            std::cout << msg_str::OOPS << msg_str::URLS_EMPTY << std::endl;
            return EXIT_FAILURE;
        }
        
        MarkovModel model = create_model_from_urls_(order, std::move(urls));
        MarkovModelSerializer serializer;

        serializer.to_stream(model, std::cout);

        return EXIT_SUCCESS;

    } catch (const std::invalid_argument &e) {
        std::cerr << msg_str::OOPS << e.what() << std::endl;
    } catch (const std::out_of_range &e) {
        std::cerr << msg_str::OOPS << e.what() << std::endl;
    } catch (const std::logic_error &e) {
        std::cerr << msg_str::OOPS << e.what() << std::endl;
    } catch (...) {
        std::cerr << msg_str::OOPS << msg_str::UNKNOWN_ERROR << std::endl;
    }

    return EXIT_FAILURE;
}

int 
App::exec_mode_(const std::string &arg)
{
    try {
        MarkovModel model = load_from_file_(arg);

        StrFilter filter;

        std::cout << msg_str::MODEL_IS_READY 
                  << "\n"
                  << msg_str::ENTER_ORDER_BEGIN 
                  << model.order() 
                  << msg_str::ENTER_ORDER_CONT
                  << msg_str::EXIT_SEQ 
                  << msg_str::ENTER_ORDER_END 
                  << "\n"
                  << msg_str::PHRASE_TO_GEN;

        for (std::string line; std::getline(std::cin, line);) {
            if (!line.empty()) {
                if (line == msg_str::EXIT_SEQ) {
                    return EXIT_SUCCESS;
                }

                State state;
                bool ok;
                std::tie(state, ok) = get_state_(
                        std::move(line), filter, model.order());

                if (ok) {
                    std::cout << msg_str::N_COUNT_TO_GEN;
                    size_t count = get_order_from_cin_();

                    if (count != std::string::npos) {
                        std::string res = 
                            model.generate(std::move(state), count);
                        std::cout << msg_str::GEN_RESULT_PROMPT << res << "\n";
                    }

                    std::cout << msg_str::PHRASE_TO_GEN;
                }
            } 
        }

        return EXIT_SUCCESS;
    } catch (const std::invalid_argument &e) {
        std::cerr << msg_str::OOPS << e.what() << std::endl;
    } catch (const std::out_of_range &e) {
        std::cerr << msg_str::OOPS << e.what() << std::endl;
    } catch (const std::logic_error &e) {
        std::cerr << msg_str::OOPS << e.what() << std::endl;
    } catch (...) {
        std::cerr << msg_str::OOPS << msg_str::UNKNOWN_ERROR << std::endl;
    }

    return EXIT_FAILURE;
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
    std::cout << msg_str::ARGS_ERROR << "\n"
              << msg_str::BUILD_PROMPT 
              << MODE_CREATE_MODEL 
              << msg_str::BUILD_PROMPT_END << "\n"
              << msg_str::RUN_PROMPT
              << MODE_EXEC_MODEL 
              << msg_str::RUN_PROMPT_END
              << std::endl;
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
        std::string cmd = CURL_CMD + url;
        auto res = pipe.read(cmd);
        auto &data = res.first;
        auto ok = res.second;

        if (ok) {
            if (!data.empty()) {
                filter.process(data, locale_);
                tutor.train(model, std::move(data), false);
            }
        } else {
            std::cerr << msg_str::OOPS 
                      << msg_str::FILE_PROC_ERROR
                      << url 
                      << std::endl;
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

size_t 
App::get_order_from_cin_() const
{
    size_t order = std::string::npos;
    std::string line;
    std::cin >> line;

    try {
        order = stoll(line);
    } catch (...) {
        order = std::string::npos;            
    }

    return order;
}

std::pair<
    State, bool
> App::get_state_(
        std::string &&line,
        const StrFilter &filter,
        size_t model_order) const
{
    trim(line);
    filter.process(line, locale_);

    State state;
    size_t n_count = 0;
    auto tokens = split_to_list(std::move(line), ' ');
    bool ok = false;

    for (auto itr = tokens.begin(); itr != tokens.end(); ++itr) {
        if (!itr->empty()) {
            state.emplace_back(*itr);
            ++n_count;
        }
    }

    if (n_count < model_order) {
        std::cout << msg_str::WARN_LESS_TOKENS_BEGIN
                  << model_order 
                  << msg_str::WARN_LESS_TOKENS_END
                  << "\n";

        ok = false;
    } else if (n_count != model_order) {
        std::cout << msg_str::WARN_MORE_TOKENS_BEGIN
                  << "\n";

        auto itr = state.begin();
        std::advance(itr, model_order);
        state.erase(
            itr,
            state.end()
        );
        ok = true;
    } else {
        ok = true;
    }

    return std::make_pair(state, ok);
}
