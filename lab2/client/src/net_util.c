#include "net_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "constants.h"

int setupTCPSocket() 
{
    int sock;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
    {
        perror("socket");
        return -1;
    }
    
    return sock;
}

ErrorCode connectToServer(const int sock, const struct sockaddr_in *const server_addr)
{
    if (connect(sock, (struct sockaddr *)server_addr, sizeof(struct sockaddr_in)) < 0) {
        return FAIL;
    }
    return SUCCESS;
}

void setupSocketTimeout(const int sock, const int seconds)
{
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}

ErrorCode setupSockAddress(struct sockaddr_in* const addr, uint16_t port, const char* const ip)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &addr->sin_addr) <= 0) {
        perror("inet_pton");
        return FAIL;
    }

    return SUCCESS;
}

ErrorCode sendSocketMessage(const int sock, const char* const msg)
{

    if (send(sock, msg, strlen(msg), NO_ARGUMENT) < 0) {
        perror("send");
        return FAIL;
    }
    
    return SUCCESS;
}

ErrorCode receiveSocketResponse(const int sock, char* const buffer,const size_t bufferSize)
{

    ssize_t len = recv(sock, buffer, bufferSize - 1, 0);
    if (len < 0) {
        perror("recv");
        return FAIL;
    }
    
    buffer[len] = '\0';
    return SUCCESS;
}

const char* getIpString(const struct sockaddr_in* const addr)
{
    return inet_ntoa(addr->sin_addr);
}

uint16_t getPort(const struct sockaddr_in* const addr)
{
    return ntohs(addr->sin_port);
} 

bool validatePort(const uint16_t port) {
    return port > MIN_PORT_VALUE && port <= MAX_PORT_VALUE;
}