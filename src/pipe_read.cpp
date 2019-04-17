#include "pipe_read.h"

#include <iostream>
#include <sstream>

namespace msg_str {
    static const char CMD_EXEC_FAIL[] = "Cmd exit unsuccessfully :";
    static const char CANNOT_PIPE[] = "Can't pipe cmd :";
}

const char PipeRead::MODE[] = "r";

std::pair<
    std::string, bool
> PipeRead::read(
        std::string cmd)
{
    bool ok = false;
    std::stringstream ss;
#   ifdef STATIC_IO_BUFFER
    static 
#   endif
    char buff[BUFF_SIZE] = {0};

    if (open_(cmd, MODE)) {
        while (!is_eof_()) {
            if (get_(buff, BUFF_SIZE) != nullptr) {
                ss << buff;
            }
        }

        if (close_() == 0) {
            ok = true;
        } else {
            log_error_(msg_str::CMD_EXEC_FAIL + cmd);
        }
    } else {
        log_error_(msg_str::CANNOT_PIPE + cmd);
    }

    return std::make_pair(ss.str(), ok);
}
