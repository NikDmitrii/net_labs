#include "server.h"
#include "options_handler.h"
#include <stdio.h>
#include "constants.h"

static void OnResponseCallback(const char* const msg, const char* const ip, const uint16_t port)
{
    printf("Msg: %s ip: %s port: %d\n", msg, ip, port);
}

static SendInfo* prepareClientInfo(const OptionsData* const options) 
{
    SendInfo* clientInfo = createInfo(options->netName, SERVER_ANSWER, options->port);
    if(clientInfo == NULL)
    {
        fprintf(stderr, "Error, create client info\n");
    }
    return clientInfo;
}

int main(int argc, char** argv){

    OptionsData options = optionsHandle(argc, argv);
    if(options.errorCode != SUCCESS){

        fprintf(stderr, "Error: %s\n", options.errorMsg);
        return options.errorCode;
    }

    SendInfo* clientInfo = prepareClientInfo(&options);
    if (clientInfo == NULL)
    {
        return FAIL;
    }

    ErrorCode result = sendBroadcast(clientInfo, OnResponseCallback);

    freeInfo(clientInfo);
    if(result == FAIL)
    {
        fprintf(stderr, "Error, sendBroadcast return FAIL\n");
    }
    return result;
}
