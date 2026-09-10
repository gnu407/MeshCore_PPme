#pragma once
#include <Arduino.h>
#include <SPIFFS.h>
#include "NodeDB.h"
#include "RTC.h"

#define WARDRIVER_ENABLED 1
#define PING_INTERVAL_MS 60000 // Send ping every 60 seconds
#define LOG_FILE_PATH "/wardrive.csv"

void initWardriver();
void logNodeToFS(const meshtastic_NodeInfo* node);
void sendActivePing();
