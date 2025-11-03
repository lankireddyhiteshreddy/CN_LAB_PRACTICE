#include "crc_common.h"

int main(){
    int receiverSock,senderSock;
    struct sockaddr_in receiverAddr,senderAddr;
    socklen_t addr1 = sizeof(senderAddr);

    receiverSock = socket(AF_INET,SOCK_STREAM,0);

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT);
    receiverAddr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(receiverSock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    bind(receiverSock,(struct sockaddr*)& receiverAddr,sizeof(receiverAddr));

    cout<<"Server sock created"<<endl;

    listen(receiverSock,1);

    senderSock = accept(receiverSock,(struct sockaddr*)& senderAddr,&addr1);

    int packetCount = 0;

    while(true){
        Packet p;
        memset(&p,0,sizeof(Packet));
        recv(senderSock,&p,sizeof(Packet),0);

        if(p.is_last_packet){
            cout<<"All packets successfully transmitted without errors"<<endl;
            break;
        }

        bool isValid = verifyCRC(&p);

        if(!isValid){
            cout<<"Invaild packet transmitted"<<endl;
            break;
        }
        else{
            cout<<"Received packet : "<<packetCount++<<endl;
        }
    }

    close(receiverSock);
    close(senderSock);
}