#include <string>
#include <list>

#include "app.h"

int
main(int argc, char *argv[])
{
    std::list<std::string> args;

    for (int i = 1; i != argc; ++i) {
        args.emplace_back(argv[i]);
    }

    App app(std::move(args));
    return app.exec();
}
