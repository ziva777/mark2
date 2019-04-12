#include "pipe_read.h"

#include <iostream>
#include <sstream>

std::pair<
    std::string, bool
> PipeRead::read(
        std::string cmd)
{
    bool ok = false;
    std::stringstream ss;

    if (open_(cmd, "r")) {
        char *s;
        static char buff[BUFF_SIZE] = {0};

        while (!is_eof_()) {
            if ((s = get_(buff, BUFF_SIZE)) != nullptr) {
                ss << buff;
            }
        }

        if (close_() == 0) {
            ok = true;
        } else {
            std::cerr << "Cmd exit unsuccessfully :" << cmd;
            std::cerr << std::endl;
        }
    } else {
        std::cerr << "Can't pipe cmd :" << cmd;
        std::cerr << std::endl;
    }

    return std::make_pair(ss.str(), ok);
}
