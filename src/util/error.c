#include "error.h"

const error throw_er(enum error_e code, const char *message)
{
    error e;
    e.code = code;
    e.message = message;

    return e;
}

const char *print_error(error e)
{
    switch (e.code) {
        case ER_SUCCESS:       return "ER_SUCCESS";
        case ER_MALLOC_FAIL:   return "ER_MALLOC_FAIL";
        case ER_INIT_ZERO_SZ:  return "ER_INIT_ZERO_SZ";
        case ER_IO_FAIL:       return "ER_IO_FAIL";
        case ER_OUT_OF_BOUNDS: return "ER_OUT_OF_BOUNDS";
        case ER_LOOKUP_FAIL:   return "ER_LOOKUP_FAIL";
        default: return "ER_UNKNOWN";
    }
}
