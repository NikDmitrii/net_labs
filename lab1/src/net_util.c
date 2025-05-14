#include "net_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

int setupBroadcastSocket() 
{
    int sock;
    
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("socket");
        return -1;
    }

    int broadcast_enable = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) 
    {
        perror("setsockopt - SO_BROADCAST");
        close(sock);
        return -1;
    }
    
    return sock;
}

void setupSocketTimeout(int sock, int seconds)
{
    struct timeval timeout;
    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}

void setupBroadcastAddress(struct sockaddr_in* addr, uint16_t port, const char* ip)
{
    memset(addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = inet_addr(ip);
}

ErrorCode sendSocketMessage(int sock, const char* msg, const struct sockaddr_in* addr)
{
    if (sendto(sock, msg, strlen(msg), 0,
               (const struct sockaddr*)addr, sizeof(*addr)) < 0) 
    {
        perror("sendto");
        return FAIL;
    }
    return SUCCESS;
}

ErrorCode receiveSocketResponse(int sock, char* buffer, size_t bufferSize,
                             struct sockaddr_in* sender_addr, socklen_t* addr_len)
{
    ssize_t recv_len = recvfrom(sock, buffer, bufferSize - 1, 0,
                               (struct sockaddr*)sender_addr, addr_len);

    if (recv_len < 0) 
    {
        return FAIL;
    }
    
    buffer[recv_len] = '\0';
    return SUCCESS;
}

const char* getIpString(const struct sockaddr_in* addr)
{
    return inet_ntoa(addr->sin_addr);
}

uint16_t getPort(const struct sockaddr_in* addr)
{
    return ntohs(addr->sin_port);
} 