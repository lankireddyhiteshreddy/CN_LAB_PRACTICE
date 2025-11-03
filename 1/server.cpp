#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#include<bits/stdc++.h>

#define PORT 8080

using namespace std;

int main(){
    int serverSock,senderSock,errorSock,receiverSock;
    struct sockaddr_in serverAddr,senderAddr,errorAddr,receiverAddr;
    socklen_t addrLen1 = sizeof(senderAddr);
    socklen_t addrLen2 = sizeof(errorAddr);
    socklen_t addrLen3 = sizeof(receiverAddr);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_family = AF_INET;

    int opt = 1;
    setsockopt(serverSock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    bind(serverSock,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

    cout<<"Server is running"<<endl;

    listen(serverSock,3);

    senderSock = accept(serverSock,(struct sockaddr*)&senderAddr,&addrLen1);
    cout<<"Sender connected"<<endl;

    

}