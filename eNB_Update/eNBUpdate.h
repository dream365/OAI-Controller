/*
 * eNBUpdate.h
 * 
 * register and update eNB's information
 */

#ifndef ENBUPDATE_H
#define ENBUPDATE_H

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

const unsigned short PORT_NUMBER = 3005;
const unsigned short UPDATE_TIME = 3;

class eNBUpdate {
public:
    eNBUpdate();
    eNBUpdate(eNBInfo* eNB);
    eNBUpdate(const eNBUpdate& orig);
    virtual ~eNBUpdate();
    
    void eNBRegisterRequest();
    void eNBUpdateRequest();
    
    void eNBRegisterResponse();
    void eNBUpdateResponse();
    
    void eNBRequest(int msgType);
    bool eNBResponse(int msgType);
    
private:
    eNBInfo* m_eNBInfo;
    Socket m_sock;
    bool m_isRegister;
    std::string m_msg;
    
    void UpdateProcess();
};

#endif /* ENBUPDATE_H */

