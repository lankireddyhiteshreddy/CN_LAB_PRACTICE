#include "protocols.h"


int main(){
    int receiverSock;
    struct sockaddr_in serverAddr;

    receiverSock = socket(AF_INET,SOCK_STREAM,0);

    serverAddr.sin_addr.s_addr = inet_addr(HOST);
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;

    connect(receiverSock,(struct sockaddr*)& serverAddr,sizeof(serverAddr));

    IPHeader ipheader;
    memset(&ipheader,0,sizeof(IPHeader));

    recv(receiverSock,&ipheader,sizeof(IPHeader),0);

    cout<<"Received"<<endl;

    uint16_t checksum = checksumModule(&ipheader);

    cout<<checksum<<endl;

    string message = "";

    if(checksum==0){
        message+="Valid checksum";
    }
    else{
        message += "Invalid checksum";
    }

    cout<<message<<endl;

    send(receiverSock,message.c_str(),message.length(),0);

    cout<<"Message sent : "<<message<<endl;

    close(receiverSock);
}