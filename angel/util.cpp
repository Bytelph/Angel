#include"util.h"
#include"log.h"

namespace angel
{
pid_t GetThreadId()
{
    return syscall(SYS_gettid);
}
uint32_t GetFiberId()
{
    return 1;
}

}
