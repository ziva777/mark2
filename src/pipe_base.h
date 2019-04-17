#ifndef __pipe_base_h__
#define __pipe_base_h__

#include <sys/types.h> 
#include <string>


class PipeBase {
public:
    virtual ~PipeBase();

protected:
    FILE *pipe_ {nullptr};

    bool open_(
            std::string cmd, 
            std::string mode);
    int close_();
    bool is_eof_() const;
    char * get_(
            char *buff,
            int buff_size);
    void log_error_(
            const std::string &msg);
};

#endif // __pipe_base_h__
