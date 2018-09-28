/*
 * CNUE.h
 * 
 * Register UE by controller's command
 */

#ifndef CNUE_H
#define CNUE_H

#include "Packet.h"
#include "Socket.h"
#include "DB.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class CNUE {
public:
    CNUE();
    CNUE(DB* db);
    CNUE(const CNUE& orig);
    virtual ~CNUE();
    
    void RegisterProcess();
    std::string Command(int type, std::string value);
    
private:
    Socket m_sock;
    Socket clnt_sock;
    DB* m_db;
};

#endif /* CNUE_H */

