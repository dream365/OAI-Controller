/*
 * eNBUpdate.cpp
 */

#include "eNBUpdate.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>
using namespace std;


eNBUpdate::eNBUpdate() {
    UpdateProcess();
}

eNBUpdate::eNBUpdate(eNBInfo* eNB) {
    m_eNBInfo = eNB;
    m_isRegister = true;
    m_msg = "";
    UpdateProcess();
}

eNBUpdate::eNBUpdate(const eNBUpdate& orig) {
}

eNBUpdate::~eNBUpdate() {
}

void eNBUpdate::UpdateProcess() {  
    
    if(!m_sock.create())
        cout << "[error] Socket Create Failed!!" << endl;

    if(m_sock.connect("163.180.118.212", 12031))
        cout << "[eNB Update] Update Socket Connect Successful" << endl;
    else
        cout << "[error] Socket Connect Failed!!" << endl;
    
    m_eNBInfo->UpdateeNBInfo();
    
    while(true) {
        
        if(m_isRegister) {
            eNBRequest(eNB_REGISTRATION_REQUEST);
            cout << PrintTime() << "[eNB Update] eNB Registration Request" << endl;

            m_isRegister = false;
            
            if(eNBResponse(eNB_REGISTRATION_RESPONSE))
                cout << PrintTime() << "[eNB Update] eNB Registration Response" << endl;
        }
        else {          
            sleep(UPDATE_TIME);
            m_eNBInfo->UpdateeNBInfo();
          
            eNBRequest(eNB_STATE_UPDATE_REQUEST);
            cout << PrintTime() << "[eNB Update] eNB Update Request" << endl;
          
            if(eNBResponse(eNB_STATE_UPDATE_RESPONSE))
                cout << PrintTime() << "[eNB Update] eNB Update Response" << endl;
        }
    }
}


void eNBUpdate::eNBRequest(int msgType) {
    
    Packet pkt;
    pkt.DecideMessageType(msgType);
    
    for(int type=eNB_ID; type <= eNB_DESCRIPTION; type++) {
        pkt.AddValue(type, m_eNBInfo->GeteNBInfo(type));
    }
    
    string msg = pkt.CreateMessage();
    
    m_sock.send(msg);
    msg.clear();
}

bool eNBUpdate::eNBResponse(int msgType) {
    
    Packet pkt;
    while(true) {
        m_sock.recv(m_msg);
            
            if(m_msg != "") {
                pkt.Parse(m_msg);
                if(pkt.m_MsgType == msgType) {
                    return true;
            }
        }
    }
    
    m_msg.clear();
}