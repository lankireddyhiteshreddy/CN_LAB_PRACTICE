#include "crc_common.h"

int main(){
    int senderSock;
    struct sockaddr_in receiverAddr;

    senderSock = socket(AF_INET,SOCK_STREAM,0);

    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT);
    receiverAddr.sin_addr.s_addr = inet_addr(HOST);

    connect(senderSock,(struct sockaddr*)& receiverAddr,sizeof(receiverAddr));

    ifstream inputFile("input.txt",ios::binary);
    cout<<"[Sender] Reading from input.txt..." << endl;
    
    int packetCount = 0;
    while(!inputFile.eof()){
        Packet packet;
        memset(&packet,0,sizeof(Packet));
        packet.is_last_packet = false;

        inputFile.read(packet.data,PACKET_SIZE);

        int bytesRead = inputFile.gcount();
        if(bytesRead == 0){
            break;
        }

        packet.bytes_in_packet = bytesRead;

        cout << "\n[Sender] Preparing Packet " << packetCount++ << " (" << bytesRead << " bytes)" << endl;

        uint16_t checksum = calculateCRC(packet.data,bytesRead);

        packet.crc_checksum = htons(checksum);

        send(senderSock,&packet,sizeof(Packet),0);
    }
    inputFile.close();

    Packet lastPacket;
    memset(&lastPacket,0,sizeof(Packet));
    lastPacket.is_last_packet = true;

    send(senderSock,&lastPacket,sizeof(Packet),0);

    close(senderSock);
}