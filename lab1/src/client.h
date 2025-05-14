#pragma once
#include "models.h"
#include "net_util.h"

// Объявление структуры как непрозрачного типа
typedef struct SendInfo SendInfo;

// Тип функции обратного вызова при получении ответа
typedef void (*OnResponse)(const char* const msg, const char* const ip, const uint16_t port);

SendInfo* createInfo(const char* const ip, const char* const msg, const uint16_t port);

ErrorCode setTimeout(SendInfo* info, const int sec);
ErrorCode setPort(SendInfo* info, const uint16_t port);
ErrorCode setMsg(SendInfo* info, const char* const msg);
ErrorCode setIp(SendInfo* info, const char* const ip);

ErrorCode sendBroadcast(const SendInfo* const info, OnResponse onResponse);

void freeInfo(SendInfo* info);
