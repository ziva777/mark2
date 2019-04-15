#include <iostream>
#include <sstream>
#include <cassert>

#include "markov_model.h"
#include "markov_model_tutor.h"
#include "markov_model_serializer.h"

void 
test_03_1()
{
    MarkovModel model(1);
    MarkovModelTutor tutor;
    MarkovModelSerializer serializer;
    std::string data = 
        "one word by one another word\n"
        "слово за слово после слово\n"
        "one two one three";
    //     order:1
    //     [two]     : [{one:1}, 1]
    //     [после]   : [{слово:1}, 1]
    //     [word]    : [{слово:1, by:1}, 2]
    //     [one]     : [{another:1, three:1, two:1, word:1}, 4]
    //     [by]      : [{one:1}, 1]
    //     [another] : [{word:1}, 1]
    //     [слово]   : [{one:1, после:1, за:1}, 3]
    //     [за]      : [{слово:1}, 1]

    tutor.train(model, std::move(data), true);
    std::stringstream ss1;
    serializer.to_stream(model, ss1);
    std::string result1 = ss1.str();

    MarkovModel model2 = serializer.from_stream(ss1);
    std::stringstream ss2;
    serializer.to_stream(model, ss2);
    std::string result2 = ss2.str();

    assert(result1 == result2);

    std::cout << "Test #03.01 PASSED" << std::endl;
}

int 
main()
{
    std::cout << "Test #03\n";
    test_03_1();
    return EXIT_SUCCESS;
}
