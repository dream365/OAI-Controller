/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CNUE.cpp
 * Author: sewon
 * 
 * Created on January 3, 2017, 11:24 AM
 */

#include "CNUE.h"

using namespace std;

CNUE::CNUE() {
    //RegisterProcess();
}

CNUE::CNUE(DB* db) {
    m_db = db;
    RegisterProcess();
}

CNUE::CNUE(const CNUE& orig) {
}

CNUE::~CNUE() {
}

void CNUE::RegisterProcess() {
    
    string msg, result;
    
    if(!m_sock.create())
        cout << "[error] Socket Create Failed!!" << endl;
    
    if(!m_sock.bind(12033))
        cout << "[error] Socket Bind Failed!!" << endl;
    
    if(!m_sock.listen())
        cout << "[error] Socket Listen Failed!!" << endl;
    
    while(1) {
        
        if(m_sock.accept(clnt_sock)) {
            cout << "[CN UE] Socket Accept Successful" << endl;
        }
        else
            cout << "[error] Socket Accept Failed!!" << endl;
        
        clnt_sock.recv(msg);
        
        if(msg != "") {
            Packet sendpkt, recvpkt;
            recvpkt.Parse(msg);
            if(recvpkt.m_MsgType == UE_REGISTRATION_REQUEST) {
                cout << PrintTime() << "[CN UE] UE Registration Request" << endl;
                msg.clear();
                result = Command(recvpkt.m_ValueType[0], recvpkt.m_Value[0]);
                
                sendpkt.DecideMessageType(UE_REGISTRATION_RESPONSE);
                sendpkt.AddValue(ACK, result);
                result.clear();
                msg = sendpkt.CreateMessage();
                clnt_sock.send(msg);
                cout << PrintTime() << "[CN UE] UE Registration Response" << endl;
                msg.clear();
            }
            else if(recvpkt.m_MsgType == DB_LIST_REQUEST) {
                cout << PrintTime() << "[CN DB] Print eNB and UE List Request" << endl;
                msg.clear();     
                
 
                sendpkt.DecideMessageType(DB_LIST_RESPONSE);
                sendpkt.AddValue(ACK, result);
                result.clear();
                msg = sendpkt.CreateMessage();
                clnt_sock.send(msg);
                cout << PrintTime() << "[CN DB] Print eNB and UE List Response" << endl;
                msg.clear();
            }
        }
        
        clnt_sock.close_socket();
    }
}

std::string CNUE::Command(int type, std::string value) {
    
    bool issuccess = true;
    string msg = "";
    string query = "";
    stringstream ss;
    
    switch(type) {
        case ADD_UE:
            if(value.length() == 15) {
                query = "INSERT INTO users (`imsi`, `msisdn`, `imei`, `imei_sv`, `ms_ps_status`, `rau_tau_timer`, `ue_ambr_ul`, `ue_ambr_dl`, `access_restriction`, `mme_cap`, `mmeidentity_idmmeidentity`, `key`, `RFSP-Index`, `urrp_mme`, `sqn`, `rand`, `OPc`) VALUES ('" + value + "',  '33638060010', NULL, NULL, 'PURGED', '120', '50000000', '100000000', '47', '0000000000', '7', 0x8BAF473F2F8FD09487CCCBD7097C6862, '1', '0', '', 0x00000000000000000000000000000000, '');";
                m_db->SendQuery(query);
                query.clear();
                int id = m_db->FindLargestKeyValue("pdn","id") + 1;
                ss << id;
                query = "INSERT INTO pdn (`id`, `apn`, `pdn_type`, `pdn_ipv4`, `pdn_ipv6`, `aggregate_ambr_ul`, `aggregate_ambr_dl`, `pgw_id`, `users_imsi`, `qci`, `priority_level`,`pre_emp_cap`,`pre_emp_vul`, `LIPA-Permissions`) VALUES ('" + ss.str() + "',  'oai.ipv4','IPV4', '0.0.0.0', '0:0:0:0:0:0:0:0', '50000000', '100000000', '3',  '" + value + "', '9', '15', 'DISABLED', 'ENABLED', 'LIPA-ONLY');";
                m_db->SendQuery(query);
                
                cout << "[CN UE] Add UE complete" << endl;
            }
            else
                issuccess = false;
            break;
        case DELETE_UE:
            if(value.length() == 15) {
                query = "DELETE FROM users WHERE imsi = " + value;
                m_db->SendQuery(query); 
                query.clear();
                query = "DELETE FROM pdn WHERE users_imsi = " + value;
                m_db->SendQuery(query); 
                
                cout << "[CN UE] Delete UE complete" << endl;
            }
            else
                issuccess = false;
            break;
        default:
            break;
    }
    
    if(issuccess) {
        msg = "[Controller] UE Registration success";
        return msg;
    }
    else {
        msg = "[error] UE Resgistration failed!!";
        return msg;
    }
}