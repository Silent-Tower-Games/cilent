#pragma once

#include <assert.h>
#include <Cilent/Misc/Assert.h>
#include <Cilent/Misc/Log.h>

#define CILENT_ASSERT(v) { \
    if (!(v)) \
    { \
        Cilent_Log_ToFile(1); \
        debug_log_type(ERROR, "ASSERTION FAILED: `%s`", #v); \
        assert(v); \
    } \
}
