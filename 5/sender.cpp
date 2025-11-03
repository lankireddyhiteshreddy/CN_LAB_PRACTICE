#include "SAWP.h"

int main(){
    int senderSock;
    struct sockaddr_in senderAddr,receiverAddr;
    socklen_t addr1 = sizeof(receiverAddr);

    senderSock = socket(AF_INET,SOCK_DGRAM,0);

    receiverAddr.sin_addr.s_addr = INADDR_ANY;
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(PORT);

    struct timeval tv;
    tv.tv_sec = TIMEOUT_SEC;
    setsockopt(senderSock,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv));

    char payload_0[PAYLOAD_SIZE] = "This is Packet 0.";
    char payload_1[PAYLOAD_SIZE] = "This is Packet 1.";

    char* payloads[] = {payload_0,payload_1,payload_0};

    int nxt_seq_to_send = 0;

    for(int i=0;i<3;i+=1){
        Packet data_packet;
        data_packet.type = DATA_TYPE;
        data_packet.seq_num = nxt_seq_to_send;
        strncpy(data_packet.data,payloads[i],PAYLOAD_SIZE);

        int expected_ack_num = (nxt_seq_to_send+1)%2;

        cout << "\n---------------------------------" << endl;
        cout << "[Sender] Sending Packet " << data_packet.seq_num << " (Attempt 1)..." << endl;

        int retransmitCount = 0;
        while(true){
            sendto(senderSock,&data_packet,sizeof(Packet),0,(struct sockaddr*)&receiverAddr,sizeof(receiverAddr));

            Packet ack_packet;
            if(recvfrom(senderSock,&ack_packet,sizeof(Packet),0,nullptr,nullptr)>=0){
                if (ack_packet.type == ACK_TYPE && ack_packet.seq_num == expected_ack_num) {
                    // Correct ACK!
                    cout << "[Sender] Received ACK " << ack_packet.seq_num << ". Packet OK." << endl;
                    break; // Exit retransmission loop
                }
                else{
                    // Wrong ACK (duplicate or corrupted)
                    cout << "[Sender] Received bad ACK (seq=" << ack_packet.seq_num << "). Ignoring." << endl;
                    // We just let it time out and resend
                }
            }
            else{
                // 5. TIMEOUT! (recvfrom() failed)
                cout << "[Sender] TIMEOUT! Resending Packet " << data_packet.seq_num << " (Attempt " << ++retransmitCount << ")..." << endl;
                // The loop will now repeat, re-sending the packet
            }
        }

        nxt_seq_to_send = (nxt_seq_to_send+1)%2;
    }
    
    cout << "\n---------------------------------" << endl;
    cout << "[Sender] All scenarios complete. Closing." << endl;
    close(senderSock);
    return 0;
}