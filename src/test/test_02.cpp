#include <iostream>
#include <cstdlib>
#include <cassert>

#include "../pipe_read.h"
#include "../str_filter.h"

void
test_02_1()
{
    std::string cmd = "echo 123";
    auto res = PipeRead{}.read(cmd);

    assert(res.second == true);
    assert(res.first == "123\n");

    std::cout << "Test #02.01 PASSED" << std::endl;
}

void
test_02_2()
{
    StrFilter filter;
    const char LOCALE[] = "ru_RU.UTF-8";
    std::string s;

    s = "hi";
    s = filter.process(std::move(s), LOCALE);
    assert(s == "hi");

    s = "HI";
    s = filter.process(std::move(s), LOCALE);
    assert(s == "hi");

    s = "Привет";
    s = filter.process(std::move(s), LOCALE);
    assert(s == "привет");

    std::cout << "Test #02.02 PASSED" << std::endl;
}

int
main()
{
    std::cout << "Test #02\n";
    test_02_1();
    test_02_2();
    return EXIT_SUCCESS;
}
