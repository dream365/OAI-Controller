/*
 * CNUpdate.h
 * 
 * Update eNB's info on CN's DB
 */

#ifndef CNUPDATE_H
#define CNUPDATE_H

#include "Socket.h"
#include "Packet.h"
#include "DB.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


class CNUpdate {
public:
    CNUpdate();
    CNUpdate(Socket* sock, DB* db);
    CNUpdate(const CNUpdate& orig);
    virtual ~CNUpdate();
    
private:
    std::string CreateRegisterQuery(Packet* pkt);
    std::string CreateUpdateQuery(Packet* pkt);
    void UpdateProcess();
    
    Socket* m_sock;
    DB* m_db;
};

#endif /* CNUPDATE_H */

