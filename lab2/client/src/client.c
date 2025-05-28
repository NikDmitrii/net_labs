#include "client.h"
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

struct SendInfo {
    Socket socket;
    char* msg;
    int timeout;
};


ErrorCode sendToServer(const SendInfo* const info, OnResponse onResponse)
{
    if(info == NULL || onResponse == NULL)
    {
        return FAIL;
    }

    int sock;
    struct sockaddr_in serverAddr;
    char buffer[MIDDLE_BUFFER_SIZE];

    if ((sock = setupTCPSocket()) < 0) 
    {
        return FAIL;
    }

    setupSockAddress(&serverAddr, info->socket.port, info->socket.ip);
    
    size_t attempts = 0;

    setupSocketTimeout(sock, info->timeout);
    while (true) 
    {
        ++attempts;
        printf("Попытка #%zu: подключение к серверу %s:%d...\n", attempts, info->socket.ip, info->socket.port);

        if(connectToServer(sock, &serverAddr) == FAIL){
            printf("Ошибка подключения, повторная попытка через %d секунд...\n", info->timeout);
            sleep(info->timeout);
            continue;
        }
        puts("Подключено к серверу");

        if (sendSocketMessage(sock, info->msg) == FAIL)
        {
            goto fail;
        }
        

        while (receiveSocketResponse(sock, buffer, MIDDLE_BUFFER_SIZE) != SUCCESS) 
        {
            printf("No answer, retry after %d seconds...\n", info->timeout);
            continue;
        } 

        onResponse(buffer, getIpString(&serverAddr), getPort(&serverAddr));
        break;
    }

    close(sock);
    return SUCCESS;

fail:
    close(sock);
    return FAIL;
}

SendInfo* createInfo(const char* const ip, const char* const msg, const uint16_t port) 
{
    if(ip == NULL || msg == NULL || !validatePort(port)){
        return NULL;
    }

    SendInfo* info = (SendInfo*)malloc(sizeof(SendInfo));
    if (info == NULL) 
    {
        goto error;
    }
    
    info->msg = NULL;
    info->socket.ip = NULL;

    if (setIp(info, ip) == FAIL)
    {
        goto error;
    }

    if (setMsg(info, msg) == FAIL)
    {
        goto error;
    }

    if (setPort(info, port) == FAIL)
    {
        goto error;
    }

    if (setTimeout(info, DEFAULT_TIMER_VALUE) == FAIL)
    {
        goto error;
    }

    return info;

error:
    freeInfo(info);
    return NULL;
}


ErrorCode setTimeout(SendInfo* info, const int sec) 
{
    if (info == NULL || sec < 0) 
    {
        return FAIL;
    }
    info->timeout = sec;
    return SUCCESS;
}

ErrorCode setPort(SendInfo* info, const uint16_t port) {
    if (info == NULL || !validatePort(port)) 
    {
        return FAIL;
    }
    info->socket.port = port;
    return SUCCESS;
}

ErrorCode setMsg(SendInfo* info, const char* const msg) 
{
    if (info == NULL || msg == NULL) 
    {
        return FAIL;
    }
    
    if (info->msg) 
    {
        free(info->msg);
        info->msg = NULL;
    }
    
    info->msg = strdup(msg);
    return info->msg ? SUCCESS : FAIL;
}

ErrorCode setIp(SendInfo* info, const char* const ip) 
{
    if (info == NULL || ip == NULL) 
    {
        return FAIL;
    }
    
    if (info->socket.ip) 
    {
        free(info->socket.ip);
        info->socket.ip = NULL;
    }
    
    info->socket.ip = strdup(ip);
    return info->socket.ip ? SUCCESS : FAIL;
}

void freeInfo(SendInfo* info) 
{
    if (info) 
    {
        if (info->socket.ip) free(info->socket.ip);
        if (info->msg) free(info->msg);
        free(info);
        info = NULL;
    }
}
