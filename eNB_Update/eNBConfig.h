/*
 * eNBConfig.h
 * 
 * configurate eNB's information
 */

#ifndef ENBCONFIG_H
#define ENBCONFIG_H

#include "Packet.h"
#include "eNBInfo.h"
#include "Socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class eNBConfig {
public:
    eNBConfig();
    eNBConfig(eNBInfo* eNB);
    eNBConfig(const eNBConfig& orig);
    virtual ~eNBConfig();
    
private:
    eNBInfo* m_eNBInfo;
    Socket m_sock;
    Socket clnt_sock;
    
    void ConfigProcess();
    std::string Command(int type, std::string value);
};

#endif /* ENBCONFIG_H */

