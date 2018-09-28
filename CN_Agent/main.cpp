#include <cstdlib>
#include <iostream>
#include <string.h>
#include <pthread.h>

#include "test.h"
#include "DB.h"
#include "CNUpdate.h"
#include "CNUE.h"

#define MAX_USER 256

using namespace std;

DB* db;

void* Client_Thread(void *arg) {
    
    Socket* clnt_sock = (Socket *)arg;
    
    CNUpdate cn(clnt_sock, db);
    
    pthread_exit(0);
}

void* Update_Thread(void *arg) {
    
    Socket serv_sock;
    Socket clnt_sock[MAX_USER];
    pthread_t client[MAX_USER];
    
    if(!serv_sock.create())
        cout << "[error] Socket Create Failed!!" << endl;
    
    if(!serv_sock.bind(12031))
        cout << "[error] Socket Bind Failed!!" << endl;
    
    if(!serv_sock.listen())
        cout << "[error] Socket Listen Failed!!" << endl;
    
    for(int i = 0; i < MAX_USER; i++) {
        
        if(serv_sock.accept(clnt_sock[i])) {
            cout << "[CN Update] Socket Accept Successful" << endl;
        }
        else
            cout << "[error] Socket Accept Failed!!" << endl;
        
        pthread_create(&client[i], NULL, &Client_Thread, (void *)&clnt_sock[i]);
    }

}

int main(int argc, char** argv) {

    db = new DB;

    try {
        Socket serv_sock;
        Socket clnt_sock[MAX_USER];
        pthread_t client[MAX_USER];

        if(!serv_sock.create())
            cout << "[error] Socket Create Failed!!" << endl;

        if(!serv_sock.bind(12031))
            cout << "[error] Socket Bind Failed!!" << endl;

        if(!serv_sock.listen())
            cout << "[error] Socket Listen Failed!!" << endl;

        for(int i = 0; i < MAX_USER; i++) {

            if(serv_sock.accept(clnt_sock[i])) {
                cout << "[CN Update] Socket Accept Successful" << endl;
            }
            else
                cout << "[error] Socket Accept Failed!!" << endl;

            pthread_create(&client[i], NULL, &Client_Thread, (void *)&clnt_sock[i]);
        }       
    }
    catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}

