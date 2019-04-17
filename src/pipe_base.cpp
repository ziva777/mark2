#include "pipe_base.h"

#include <sys/wait.h>
#include <errno.h>

PipeBase::~PipeBase()
{
    if (pipe_) {
        close_();
    }
}

bool 
PipeBase::open_(
        std::string cmd, 
        std::string mode)
{
    return (pipe_ = popen(cmd.c_str(), mode.c_str())) != nullptr;
}

int 
PipeBase::close_()
{
    int pid = pclose(pipe_);
    pipe_ = nullptr;
    return pid;
}

bool 
PipeBase::is_eof_() const
{
    return feof(pipe_);
}

char * 
PipeBase::get_(
        char *buff,
        int buff_size)
{
    return fgets(buff, buff_size, pipe_);
}

void 
PipeBase::log_error_(
        const std::string &msg)
{

}
