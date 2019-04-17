#include <iostream>
#include <cstdlib>
#include <cassert>
#include <numeric>
#include <string>
#include <list>

#include "../misc.h"


void
test_01_1()
{
    std::list<std::string> w = {
        {"one"}, {"two"}, {"three"}
    };
    std::string t1 = "one,two,three";
    std::string t2 = "one.two.three";
    std::string t3 = "one two three";

    std::string r1 = std::accumulate(
        w.begin(), w.end(), std::string(),
        StringJoin{","}
    );
    std::string r2 = std::accumulate(
        w.begin(), w.end(), std::string(),
        StringJoin{"."}
    );
    std::string r3 = std::accumulate(
        w.begin(), w.end(), std::string(),
        StringJoin{" "}
    );

    assert(t1 == r1);
    assert(t2 == r2);
    assert(t3 == r3);

    std::cout << "Test #01.01 PASSED" << std::endl;
}

void 
test_01_2()
{
    assert("one" == extract("[one]", '[', ']'));
    assert("one" == extract("[one] ", '[', ']'));
    assert("one" == extract(" [one]", '[', ']'));
    assert("one" == extract(" [one] ", '[', ']'));
    assert("one" == extract("  [one]  ", '[', ']'));
    assert("[one" == extract("  [[one]  ", '[', ']'));
    assert("one]" == extract("  [one]]  ", '[', ']'));
    assert("[one]" == extract("  [[one]]  ", '[', ']'));

    std::cout << "Test #01.02 PASSED" << std::endl;
}

void 
test_01_3()
{
    std::pair<std::string, std::string> res;

    res = split_in_pos("1:3456", 1);
    assert(res.first == "1" && res.second == "3456");

    res = split_in_pos("1:3456", std::string::npos);
    assert(res.first == "1:3456" && res.second == "");

    std::cout << "Test #01.03 PASSED" << std::endl;
}

void 
test_01_4()
{
    std::pair<std::string, std::string> res;

    res = split_left("1:2:3:4", ':');
    assert(res.first == "1" && res.second == "2:3:4");
    res = split_left("1 2 3 4", ' ');
    assert(res.first == "1" && res.second == "2 3 4");
    res = split_left("1 2 3 4", ':');
    assert(res.first == "1 2 3 4" && res.second == "");


    res = split_right("1:2:3:4", ':');
    assert(res.first == "1:2:3" && res.second == "4");
    res = split_right("1 2 3 4", ' ');
    assert(res.first == "1 2 3" && res.second == "4");
    res = split_right("1 2 3 4", ':');
    assert(res.first == "1 2 3 4" && res.second == "");

    std::cout << "Test #01.04 PASSED" << std::endl;
}

void 
test_01_5()
{
    std::list<std::string> res;
    std::list<std::string>::iterator itr;

    res = split_to_list("1 2 3 4", ' ');
    assert(res.size() == 4);
    itr = res.begin();
    assert(*itr++ == "1");
    assert(*itr++ == "2");
    assert(*itr++ == "3");
    assert(*itr++ == "4");

    res = split_to_list("one two three four five", ' ');
    assert(res.size() == 5);
    itr = res.begin();
    assert(*itr++ == "one");
    assert(*itr++ == "two");
    assert(*itr++ == "three");
    assert(*itr++ == "four");
    assert(*itr++ == "five");

    std::cout << "Test #01.05 PASSED" << std::endl;
}

void 
test_01_6()
{
    std::string s;

    // ltrim
    s = "some stuff";
    ltrim(s);
    assert(s == "some stuff");

    s = "   some stuff";
    ltrim(s);
    assert(s == "some stuff");

    s = "  some stuff   ";
    ltrim(s);
    assert(s == "some stuff   ");

    // rtrim
    s = "some stuff";
    rtrim(s);
    assert(s == "some stuff");

    s = "some stuff   ";
    rtrim(s);
    assert(s == "some stuff");

    s = "   some stuff   ";
    rtrim(s);
    assert(s == "   some stuff");

    // trim
    s = "some stuff";
    trim(s);
    assert(s == "some stuff");

    s = "some stuff   ";
    trim(s);
    assert(s == "some stuff");

    s = "   some stuff   ";
    trim(s);
    assert(s == "some stuff");

    std::cout << "Test #01.06 PASSED" << std::endl;
}

int
main()
{
    std::cout << "Test #01\n";
    test_01_1();
    test_01_2();
    test_01_3();
    test_01_4();
    test_01_5();
    test_01_6();
    return EXIT_SUCCESS;
}
