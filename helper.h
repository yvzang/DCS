#ifndef __HELPER_H__
#define __HELPER_H__
#include "network.h"
#include "task_manager.h"

WifiManager* WifiManagerGetInstance();

void readPLCConfigFile(std::string & plcIP);
void writePLCConfigFile(const std::string & ipaddr);

#endif