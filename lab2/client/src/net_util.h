#pragma once
#include "models.h"
#include <sys/socket.h>
#include <netinet/in.h>

int setupTCPSocket();

void setupSocketTimeout(const int sock, const int seconds);

ErrorCode setupSockAddress(struct sockaddr_in* const addr, uint16_t port, const char* const ip);

ErrorCode connectToServer(const int sock, const struct sockaddr_in* const addr);

ErrorCode sendSocketMessage(int sock, const char* const msg);

ErrorCode receiveSocketResponse(const int sock, char* buffer,const size_t bufferSize);

const char* getIpString(const struct sockaddr_in* const addr);

uint16_t getPort(const struct sockaddr_in* const addr); 

bool validatePort(const uint16_t port);