#include "protocols.h"


int main(){
    int serverSock,senderSock,receiverSock;
    struct sockaddr_in serverAddr,senderAddr,receiverAddr;
    socklen_t addrLen1 = sizeof(senderAddr);
    socklen_t addrLen3 = sizeof(receiverAddr);

    serverSock = socket(AF_INET,SOCK_STREAM,0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;

    int opt = 1;
    setsockopt(serverSock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    bind(serverSock,(struct sockaddr*)& serverAddr,sizeof(serverAddr));

    listen(serverSock,3);

    senderSock = accept(serverSock,(struct sockaddr*)& senderAddr,&addrLen1);
    receiverSock = accept(serverSock,(struct sockaddr*)& receiverAddr,&addrLen3);

    IPHeader ipheader;
    memset(&ipheader,0,sizeof(IPHeader));

    recv(senderSock,&ipheader,sizeof(IPHeader),0);

    cout<<"Received"<<endl;

    send(receiverSock,&ipheader,sizeof(IPHeader),0);

    cout<<"Sent to receiver"<<endl;
    
    char buffer[1024];

    int len = recv(receiverSock,buffer,sizeof(buffer),0);
    buffer[len] = '\0';
    string message(buffer);

    send(senderSock,message.c_str(),message.length(),0);

    close(senderSock);
    close(receiverSock);
    close(serverSock);
}