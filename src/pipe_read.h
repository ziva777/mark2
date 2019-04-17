#ifndef __pipe_read_h__
#define __pipe_read_h__

#include <utility>
#include <string>

#include "pipe_base.h"

#define STATIC_IO_BUFFER


class PipeRead : public PipeBase {
public:
    std::pair<
        std::string, bool
    > read(
            std::string cmd);
private:
    static const int BUFF_SIZE = 1024;
    static const char MODE[];
};


#endif // __pipe_read_h__
