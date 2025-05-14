#pragma once
#include <stdbool.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum{
    SUCCESS = EXIT_SUCCESS,
    FAIL = EXIT_FAILURE
} ErrorCode;

typedef struct{
    bool nOpt;
    bool pOpt;
} OptFlags;

typedef struct{
    uint16_t port;
    char* ip;
} Socket;

typedef struct{
    const char* netName;
    uint16_t port;
    ErrorCode errorCode;
    char errorMsg[256];
} OptionsData;
