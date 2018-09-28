/*
 * eNBInfo.h
 * 
 * set and update eNB's information
 */

#ifndef ENBINFO_H
#define ENBINFO_H
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "PacketType.h"

const std::string ENB_CONFIG_PATH = "/home/sewon/openairinterface5g/targets/PROJECTS/GENERIC-LTE-EPC/CONF/enb.band7.tm1.exmimo2.conf";
const std::string ENB_PATH = "/home/sewon/openairinterface5g/cmake_targets/lte_build_oai/build/lte-softmodem";

class eNBInfo {
public:
    eNBInfo();
    eNBInfo(const eNBInfo& orig);
    virtual ~eNBInfo();
    
    void SeteNBInfo(int type, std::string value);
    std::string GeteNBInfo(int type);
    void UpdateeNBInfo();
    void ShoweNBInfo();
    
    bool SeteNBName(std::string name);
    bool SeteNBMCC(std::string mcc);
    bool SeteNBMNC(std::string mnc);
    
    bool CheckeNBstart();
    void StarteNB();
    void StopeNB();
private:
    std::string m_ID;
    std::string m_IP;
    std::string m_Name;
    std::string m_MCC;
    std::string m_MNC;
    std::string m_Description;
    std::string m_pid;
};

#endif /* ENBINFO_H */

