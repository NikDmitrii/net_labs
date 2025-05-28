#pragma once
#include "models.h"
#include <sys/socket.h>
#include <netinet/in.h>

int setupBroadcastSocket();

void setupSocketTimeout(const int sock, const int seconds);

ErrorCode setupSockAddress(struct sockaddr_in* const addr, uint16_t port, const char* const ip);

ErrorCode sendSocketMessage(int sock, const char* const msg, const struct sockaddr_in* const addr);

ErrorCode receiveSocketResponse(const int sock, char* buffer,const size_t bufferSize,
                             const struct sockaddr_in* const senderAddr,socklen_t* const addrLen);

const char* getIpString(const struct sockaddr_in* const addr);

uint16_t getPort(const struct sockaddr_in* const addr); 