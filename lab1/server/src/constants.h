#pragma once
#include <stddef.h>

//Переменные проще отлаживать, чем макросы из getopt.h
extern const int NO_ARGUMENT;
extern const int REQUIRED_ARGUMENT;
extern const int OPTIONAL_ARGUMENT;
extern const int MAX_PORT_VALUE;
extern const int DEFAULT_TIMER_VALUE;
extern const char* const SERVER_ANSWER;
extern const size_t MIDDLE_BUFFER_SIZE;
#define NO_INDEX NULL

typedef enum{
    NO_FLAG_VALUE = 0,
    USE_FLAG = 1
} LongOptFlag;