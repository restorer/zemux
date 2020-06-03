#ifndef TEST_SUITE__TEST
#define TEST_SUITE__TEST

#include <zemux_core/non_copyable.h>

class Test : private zemux::NonCopyable {
public:

    virtual ~Test() {}
    virtual bool run() = 0;
};

#endif
