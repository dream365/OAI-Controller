/*
 * CNUpdate.cpp
 */

#include "CNUpdate.h"

using namespace std;

CNUpdate::CNUpdate() {
}

CNUpdate::CNUpdate(Socket* sock, DB* db) {
    m_sock = sock;
    m_db = db;
    UpdateProcess();
}

CNUpdate::CNUpdate(const CNUpdate& orig) {
}

CNUpdate::~CNUpdate() {
}

void CNUpdate::UpdateProcess() {
    
    string msg; 
    Packet *pkt;
        
    while(true) {
        pkt = new Packet();
        m_sock->recv(msg);
        
        if(msg != "") {
            
            pkt->Parse(msg);
            switch(pkt->m_MsgType) {
                case eNB_REGISTRATION_REQUEST:
                    cout << PrintTime() << "[CN Update] eNB Registration Request" << endl;
                    m_db->SendQuery(CreateRegisterQuery(pkt));
                    break;
                case eNB_STATE_UPDATE_REQUEST:
                    cout << PrintTime() << "[CN Update] eNB Update Request" << endl;
                    m_db->SendQuery(CreateUpdateQuery(pkt));
                    break;
                default:
                    break;
            }
        }
        
        msg.clear();
        delete pkt;
    }
}

std::string CNUpdate::CreateRegisterQuery(Packet* pkt) {
    string query;
    Packet pkt2;
    
    query.append("INSERT INTO eNBs (");
    
    for(int i = 0; i < pkt->m_paramNo; i++) {
        query.append(ValueTypeToString(pkt->m_ValueType[i]));
        query.append(", ");
    }
    
    query.append("MME_ID");
    query.append(") VALUES ('");
    
    for(int i = 0; i <pkt->m_paramNo; i++) {
        query.append(pkt->m_Value[i]);
        query.append("','");
    }
    
    query.append("7');");
    
    pkt2.DecideMessageType(eNB_REGISTRATION_RESPONSE);
    m_sock->send(pkt2.CreateMessage());
    
    cout << PrintTime() << "[CN Update] eNB Registration Response" << endl;
    
    return query;
}

std::string CNUpdate::CreateUpdateQuery(Packet* pkt) {
    string query;
    Packet pkt2;
    
    query.append("INSERT INTO eNBs (");
    
    for(int i = 0; i < pkt->m_paramNo; i++) {
        query.append(ValueTypeToString(pkt->m_ValueType[i]));
        query.append(", ");
    }
    
    query.append("MME_ID");
    query.append(") VALUES ('");
    
    for(int i = 0; i <pkt->m_paramNo; i++) {
        query.append(pkt->m_Value[i]);
        query.append("','");
    }
    
    query.append("7') ON DUPLICATE KEY UPDATE ");
    
    for(int i = 1; i < pkt->m_paramNo; i++) {
        query.append(ValueTypeToString(pkt->m_ValueType[i]));
        query.append("='");
        query.append(pkt->m_Value[i]);
        query.append("',");
    }
    
    query.append("MME_ID='7';");
    
    pkt2.DecideMessageType(eNB_STATE_UPDATE_RESPONSE);
    m_sock->send(pkt2.CreateMessage());
    
    cout << PrintTime() << "[CN Update] eNB Update Response" << endl;
    
    return query;
}