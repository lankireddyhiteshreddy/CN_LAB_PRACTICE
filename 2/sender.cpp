#include "protocols.h"

int main(){
    int senderSock;
    struct sockaddr_in serverAddr;

    senderSock = socket(AF_INET,SOCK_STREAM,0);

    serverAddr.sin_addr.s_addr = inet_addr(HOST);
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;

    connect(senderSock,(struct sockaddr*)& serverAddr,sizeof(serverAddr));

    IPHeader ipheader;
    
    memset(&ipheader,0,sizeof(IPHeader));

    ipheader.ver_hlen = 0x45;
    ipheader.service_type = 0x00;
    ipheader.total_length = htons(20);

    ipheader.identification = htons(0x1234);
    ipheader.flags_fragmentation = htons(0x2334);

    ipheader.ttl = 64;
    ipheader.protocol = 6;
    ipheader.checksum = 0;

    ipheader.source_ip = inet_addr("192.169.1.1");

    ipheader.dest_ip = inet_addr("10.1.1.1");

    uint16_t checksumVal = checksumModule(&ipheader);

    ipheader.checksum = htons(checksumVal);

    send(senderSock,&ipheader,sizeof(IPHeader),0);

    cout<<"Sent successfully"<<endl;

    char buffer[1024];
    int len = recv(senderSock,buffer,sizeof(buffer),0);
    buffer[len] = '\0';
    string message(buffer);

    cout<<"Message from receiever : "<<message<<endl;

    close(senderSock);
}