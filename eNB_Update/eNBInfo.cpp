/*
 * eNBInfo.cpp
 */


#include "eNBInfo.h"
using namespace std;

eNBInfo::eNBInfo() {
    m_pid = "0";
}

eNBInfo::eNBInfo(const eNBInfo& orig) {
}

eNBInfo::~eNBInfo() {
}

void eNBInfo::SeteNBInfo(int type, std::string value) {
    
    switch(type) {
        case eNB_ID:
            this->m_ID.assign(value);
            break;       
        case eNB_IP:
            this->m_IP.assign(value);
            break;
        case eNB_NAME:
            this->m_Name.assign(value);
            break;
        case eNB_MCC:
            this->m_MCC.assign(value);
            break;
        case eNB_MNC:
            this->m_MNC.assign(value);
            break;
        case eNB_DESCRIPTION:
            this->m_Description.assign(value);
            break;
        default:
            break;
    }
            
}

std::string eNBInfo::GeteNBInfo(int type) {
    
    switch(type) {
        case eNB_ID:
            return this->m_ID;
        case eNB_IP:
            return this->m_IP;
        case eNB_NAME:
            return this->m_Name;
        case eNB_MCC:
            return this->m_MCC;
        case eNB_MNC:
            return this->m_MNC;
        case eNB_DESCRIPTION:
            return this->m_Description;
        default:
            break;
    }
    
}

void eNBInfo::UpdateeNBInfo() {
    
    string temp = "";
    std::string output = "";
    ifstream in(ENB_CONFIG_PATH.c_str());
    FILE * fp;
    char buf[64];
    
    fp = popen("ifconfig eth0 | grep HWaddr", "r");
    while(!feof(fp)) {
        fread(buf, 1, 64, fp);
    }
    temp.assign(buf);
    output = temp.substr(temp.find("HWaddr")+7, 17);
    
    m_ID = output;
    output.clear();
    temp.clear();
    pclose(fp);
    
    fp = popen("ifconfig eth0 | grep inet", "r");
    fread(buf, 1, 64, fp);
    temp.assign(buf);
    output = temp.substr(temp.find("addr:")+5);
    output.erase(output.find(" "));
    
    this->m_IP = output;
    output.clear();
    temp.clear();
    pclose(fp);
    
    if(in.is_open()) {
        while(!in.eof()) {
            getline(in, temp);
            
            if((temp.find("eNB_name  =  \"") != std::string::npos)) {
                output = temp.substr(temp.find("eNB_name  =  \"") + 14);
                output.erase(output.find("\""),2);
                this->m_Name = output;
            }
            else if((temp.find("mobile_country_code =  \"") != std::string::npos)) {
                output = temp.substr(temp.find("mobile_country_code =  \"") + 24);
                output.erase(output.find("\""),2);
                this->m_MCC = output;
            }
            else if((temp.find("mobile_network_code =  \"") != std::string::npos)) {
                output = temp.substr(temp.find("mobile_network_code =  \"") + 24);
                output.erase(output.find("\""),2);
                this->m_MNC = output;
            }
            
            output.clear();
            temp.clear();
        }
        
    }
    else {
        cout << "[error] eNB configuration file open failed!!";
    }
    
    m_Description.assign("Mobile Convergence Lab eNB 1");
    
}

void eNBInfo::ShoweNBInfo() {
    cout << "eNB Status" << endl;
    cout << setw(15) << left << "Name" << this->m_Name << endl;
    cout << setw(15) << left << "MCC" << this->m_MCC << endl;
    cout << setw(15) << left << "MNC" << this->m_MNC << endl; 
}

bool eNBInfo::SeteNBName(std::string name) {
    
    string str = "";
    
    if(name.length() > 0) {
        str = "sed -i 's/eNB_name  =  \"[a-zA-Z_]*\"/eNB_name  =  \"" + name + "\"/g' " + ENB_CONFIG_PATH;
        system(str.c_str());
        return true;
    }
    else
        return false;
}

bool eNBInfo::SeteNBMCC(std::string mcc) {
    
    int imcc = atoi(mcc.c_str());
    string str = "";
    
    if(0 <= imcc && imcc <= 999) {
        str = "sed -i 's/mobile_country_code =  \"[0-9]*\"/mobile_country_code =  \"" + mcc + "\"/g' " + ENB_CONFIG_PATH;
        system(str.c_str());
        return true;
    }
    else
        return false;
}

bool eNBInfo::SeteNBMNC(std::string mnc) {
    
    int imnc = atoi(mnc.c_str());
    string str = "";
    
    if(0 <= imnc && imnc <= 99) {
        str = "sed -i 's/mobile_network_code =  \"[0-9]*\"/mobile_country_code =  \"" + mnc + "\"/g' " + ENB_CONFIG_PATH;
        system(str.c_str());
        return true;
    }
    else
        return false;
}

bool eNBInfo::CheckeNBstart() {
    FILE * fp;
    char buf[128];
    string temp;
    ostringstream output;
    int bytes;
    
    fp = popen("ps -eo pid,comm |grep 'lte-softmodem' |grep -v 'grep'", "r");
    while(!feof(fp)) {
        bytes = fread(buf, 1, 128, fp);
        output.write(buf, bytes);
    }
    temp = output.str();
    temp = temp.substr(0,temp.find(" "));
    m_pid = temp;
    
    if(atoi(m_pid.c_str()) != 0)
        return true;
    else
        return false;
}

void eNBInfo::StarteNB() {
    string str = "sudo -E " + ENB_PATH + " -O " + ENB_CONFIG_PATH + " -d -W -S";
    system(str.c_str());
}

void eNBInfo::StopeNB() {
    string str = "sudo kill " + m_pid;
    system(str.c_str());
}