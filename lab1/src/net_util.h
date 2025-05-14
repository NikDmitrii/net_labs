#pragma once
#include "models.h"
#include <sys/socket.h>
#include <netinet/in.h>

int setupBroadcastSocket();

void setupSocketTimeout(int sock, int seconds);

void setupBroadcastAddress(struct sockaddr_in* addr, uint16_t port, const char* ip);

ErrorCode sendSocketMessage(int sock, const char* msg, const struct sockaddr_in* addr);

ErrorCode receiveSocketResponse(int sock, char* buffer, size_t bufferSize,
                             struct sockaddr_in* sender_addr, socklen_t* addr_len);

const char* getIpString(const struct sockaddr_in* addr);

uint16_t getPort(const struct sockaddr_in* addr); 