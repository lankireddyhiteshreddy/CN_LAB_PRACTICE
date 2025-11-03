#ifndef STOP_AND_WAIT_PROTOCOL_H
#define STOP_AND_WAIT_PROTOCOL_H

#include<bits/stdc++.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

using namespace std;

#define PORT 8080
#define HOST "127.0.0.1"

#define PAYLOAD_SIZE 100
#define DATA_TYPE 1
#define ACK_TYPE 2
#define TIMEOUT_SEC 2

typedef struct Packet{
    int type;
    int seq_num;
    char data[PAYLOAD_SIZE];
}Packet;

#endif