#include "options_handler.h"
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#include "constants.h"
#include <stdbool.h>
#include <sys/types.h>
#include <stdlib.h>
#include "net_util.h"

static ErrorCode localValidatePort(long port, OptionsData* options) {
    if (!validatePort(port)) {
        options->errorCode = FAIL;
        (void)strcpy(options->errorMsg, "Error, invalid port number");
        return FAIL;
    }
    return SUCCESS;
}

OptionsData optionsHandle(const int argc, char** argv)
{
    OptionsData options = {
        .netName = NULL,
        .port = 0,
        .errorCode = SUCCESS,
        .errorMsg = {0}
    };

    struct option longOptions[] = {
        {"net", REQUIRED_ARGUMENT, NO_FLAG_VALUE, 'n'},
        {"port", REQUIRED_ARGUMENT, NO_FLAG_VALUE, 'p'},
        {0, 0, 0, 0}
    };

    OptFlags flags = {
        .nOpt = false,
        .pOpt = false,
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "n:p:", longOptions, NO_INDEX)) != -1)
    {
        switch(opt)
        {
        case 'n':
        {
            flags.nOpt = true;
            options.netName = optarg;
            break;
        }
        case 'p':
        {
            flags.pOpt = true;
            char* endptr;
            long val = strtol(optarg, &endptr, 10);
            
            if (*endptr != '\0' || localValidatePort(val, &options) == FAIL) {
                return options;
            }
            
            options.port = (uint16_t)val;
            break;
        }
        default:
        {
            options.errorCode = FAIL;
            (void)strcpy(options.errorMsg, "Error, unknown option");
            return options;
        }
        }
    }

    if(!flags.nOpt || !flags.pOpt)
    {
        options.errorCode = FAIL;
        (void)strcpy(options.errorMsg, "Error, both --net and --port options are required");
        return options;
    }

    return options;
}
