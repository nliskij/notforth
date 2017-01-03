#ifndef FORTH_ERROR_H
#define FORTH_ERROR_H
enum error_e {
    ER_SUCCESS = 0, // no error
    ER_MALLOC_FAIL, // not enough memory for malloc
    ER_INIT_ZERO_SZ, // initialize size 0 struct
    ER_IO_FAIL, // getline failed
    ER_OUT_OF_BOUNDS, // attempted to access out of bounds mem
    ER_LOOKUP_FAIL
};

typedef struct error_t {
    const char *message;
    enum error_e code;
} error;

const error throw_er(enum error_e code, const char *message);
const char *print_error(error e);

#endif
