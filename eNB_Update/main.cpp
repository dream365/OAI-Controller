/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: sewon
 *
 * Created on December 1, 2016, 11:00 AM
 */


#include <cstdlib>
#include <signal.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fstream>

#include "eNBInfo.h"
#include "eNBUpdate.h"
#include "eNBConfig.h"
#include "test.h"

using namespace std;

eNBInfo* eNB;

void* Update_Thread(void *arg) {
    eNBUpdate eU(eNB);
    pthread_exit(0);
}

void* Config_Thread(void *arg) {
    eNBConfig eC(eNB);
    pthread_exit(0);
}

int main(int argc, char** argv) {
    
    eNB = new eNBInfo;

    try
    {
        pthread_t update;	
        pthread_t config;
	
	pthread_create(&config, NULL, &Config_Thread, (void *)NULL);
	pthread_create(&update, NULL, &Update_Thread, (void *)NULL);
                
        pthread_join(config, NULL);
        pthread_join(update, NULL);
	}
	catch(std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
    
    return 0;
}

