/*
 * eNBConfig.cpp
 */
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>
#include "eNBConfig.h"

using namespace std;

eNBConfig::eNBConfig() {
}

eNBConfig::eNBConfig(eNBInfo* eNB) {
    m_eNBInfo = eNB;
    ConfigProcess();
}

eNBConfig::eNBConfig(const eNBConfig& orig) {
}

eNBConfig::~eNBConfig() {
}

void eNBConfig::ConfigProcess() {
    
    string msg, result;
    
    if(!m_sock.create())
        cout << "[error] Socket Create Failed!!" << endl;
    
    if(!m_sock.bind(12032))
        cout << "[error] Socket Bind Failed!!" << endl;
    
    if(!m_sock.listen())
        cout << "[error] Socket Listen Failed!!" << endl;
    
    while(1) {
        
        if(m_sock.accept(clnt_sock)) {
            cout << "[eNB Config] Socket Accept Successful" << endl;
        }
        else
            cout << "[error] Socket Accept Failed!!" << endl;
                
        clnt_sock.recv(msg);
        
        if(msg != "") {
            Packet sendpkt, recvpkt;
            recvpkt.Parse(msg);
            if(recvpkt.m_MsgType == eNB_STATE_CONFIGURE_REQUEST) {
                cout << PrintTime() << "[eNB Config] eNB Configure Request" << endl;
                msg.clear();
                result = Command(recvpkt.m_ValueType[0], recvpkt.m_Value[0]);
                
                sendpkt.DecideMessageType(eNB_STATE_CONFIGURE_RESPONSE);
                sendpkt.AddValue(ACK, result);
                result.clear();
                msg = sendpkt.CreateMessage();
                clnt_sock.send(msg);
                cout << PrintTime() << "[eNB Config] eNB Configure Response" << endl;
                msg.clear();
            }
        }
        
        clnt_sock.close_socket();
    }
}

std::string eNBConfig::Command(int type, std::string value) {
    
    bool issuccess = true;
    string msg = "";
    
    switch(type) {
        case START_eNB:
            if(issuccess = !m_eNBInfo->CheckeNBstart())
                m_eNBInfo->StarteNB();
            else {
                msg = "[error] eNB is already start!!";
                cout << msg << endl;
            }
            break;
        case STOP_eNB:
            if(issuccess = m_eNBInfo->CheckeNBstart())
                m_eNBInfo->StopeNB();
            else {
                msg = "[error] eNB is already stop!!";
                cout << msg << endl;
            }
            break;
        case CHANGE_NAME:
            if(m_eNBInfo->CheckeNBstart())
                m_eNBInfo->StopeNB();
                        
            if(issuccess = m_eNBInfo->SeteNBName(value)) {
                cout << "[eNB Config] Set eNB's name complete" << endl;
            }
            else {
                msg = "[error] Set eNB's name failed!!";
                cout << msg << endl;
            }
            break;
        case CHANGE_MCC:
            if(m_eNBInfo->CheckeNBstart())
                m_eNBInfo->StopeNB();
            
            if(issuccess = m_eNBInfo->SeteNBMCC(value)) {
                cout << "[eNB Config] Set eNB's MCC complete" << endl;
            }
            else {
                msg = "[error] Set eNB's MCC failed!!";
                cout << msg << endl;
            }
            break;
        case CHANGE_MNC:
            if(m_eNBInfo->CheckeNBstart())
                m_eNBInfo->StopeNB();
            
            if(issuccess = m_eNBInfo->SeteNBMNC(value)) {
                cout << "[eNB Config] Set eNB's MNC complete" << endl;
            }
            else {
                msg = "[error] Set eNB's MNC failed!!";
                cout << msg << endl;
            }
            break;
        case GET_STATUS:
            break;
        default:
            break;
    }
    
    if(issuccess) {
        m_eNBInfo->UpdateeNBInfo();
        m_eNBInfo->ShoweNBInfo();
        
        msg = "[Controller] \nNAME: " + m_eNBInfo->GeteNBInfo(eNB_NAME) + "\nMCC: " + m_eNBInfo->GeteNBInfo(eNB_MCC) + "\nMNC: " + m_eNBInfo->GeteNBInfo(eNB_MNC);
        return msg;
    }
    else
        return msg;
}
