#include "net_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include "constants.h"

int setupBroadcastSocket() 
{
    int sock;
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) 
    {
        perror("socket");
        return -1;
    }

    int broadcastEnable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) 
    {
        perror("setsockopt - SO_BROADCAST");
        close(sock);
        return -1;
    }
    
    return sock;
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

ErrorCode sendSocketMessage(const int sock, const char* const msg, const struct sockaddr_in* const addr)
{
    if (sendto(sock, msg, strlen(msg), 0,
               (const struct sockaddr*)addr, sizeof(*addr)) < 0) 
    {
        perror("sendto");
        return FAIL;
    }
    return SUCCESS;
}

ErrorCode receiveSocketResponse(const int sock, char* const buffer,const size_t bufferSize,
                             const struct sockaddr_in* const senderAddr,socklen_t* const addrLen)
{
    ssize_t recvLen = recvfrom(sock, buffer, bufferSize - 1, 0,
                               (struct sockaddr*)senderAddr, addrLen);

    if (recvLen < 0) 
    {
        return FAIL;
    }
    
    buffer[recvLen] = '\0';
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