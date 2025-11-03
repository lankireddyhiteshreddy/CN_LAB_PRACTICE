#include "SAWP.h"

int main() {
    int receiver_sock;
    struct sockaddr_in receiver_addr, sender_addr;
    socklen_t sender_len = sizeof(sender_addr);
    
    receiver_sock = socket(AF_INET, SOCK_DGRAM, 0);

    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(PORT);
    receiver_addr.sin_addr.s_addr = INADDR_ANY; 

    // 3. Bind the socket to the port
    bind(receiver_sock, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr));

    cout << "[Receiver] Waiting for packets on port " << PORT << "..." << endl;

    int expected_seq_num = 0;
    int packet_receive_counter = 0; 

    while (true) {
        Packet data_packet;
        
        recvfrom(receiver_sock, &data_packet, sizeof(Packet), 0, (struct sockaddr*)&sender_addr, &sender_len);

        packet_receive_counter++;

        if (data_packet.type != DATA_TYPE) {
            continue; 
        }

        cout << "\n[Receiver] Received Packet " << data_packet.seq_num 
             << " (Total packets seen: " << packet_receive_counter << ")" << endl;

        // --- SCENARIO 2: Simulate Packet 1 Loss ---
        // This is the *second* packet we've seen, and it's Pkt 1 (the one we want)
        if (packet_receive_counter == 2 && data_packet.seq_num == 1) {
            cout << "[Receiver] *** SIMULATING PACKET 1 LOSS. Dropping, no ACK. ***" << endl;
            continue; // Drop the packet, send no ACK. Sender will time out.
        }

        // --- Check for Correct vs. Duplicate Packet ---
        int ack_num_to_send = 0;
        
        if (data_packet.seq_num == expected_seq_num) {
            // --- This is the correct packet! ---
            cout << "[Receiver] Packet OK. " << data_packet.data << endl;
            
            // Prepare the ACK for the *next* packet
            ack_num_to_send = (expected_seq_num + 1) % 2;
            
            // Move our window
            expected_seq_num = (expected_seq_num + 1) % 2;

            // --- SCENARIO 3: Simulate ACK 1 Loss ---
            // This is the *fourth* packet we've seen.
            // (1=Pkt0, 2=Pkt1-lost, 3=Pkt1-resent, 4=Pkt0-resent)
            if (packet_receive_counter == 4) {
                 cout << "[Receiver] *** SIMULATING ACK 1 LOSS. Dropping ACK. ***" << endl;
                 continue; // Don't send the ACK. Sender will time out.
            }

        } else {
            // --- This is a duplicate packet! ---
            // The sender timed out and resent a packet we already ACKed.
            cout << "[Receiver] Duplicate Packet " << data_packet.seq_num 
                 << ". Discarding." << endl;
                 
            // Send the ACK for the packet we are *still* waiting for.
            // (This is the same as resending the *last* good ACK).
            ack_num_to_send = expected_seq_num;
            cout << "[Receiver] Resending ACK " << ack_num_to_send << "..." << endl;
        }

        // --- Send the ACK ---
        Packet ack_packet;
        ack_packet.type = ACK_TYPE;
        ack_packet.seq_num = ack_num_to_send;
        
        sendto(receiver_sock, &ack_packet, sizeof(Packet), 0, (struct sockaddr*)&sender_addr, sender_len);
        
        // Stop the server after the 3rd scenario (duplicate packet) is handled
        if (packet_receive_counter == 5) {
             cout << "\n[Receiver] All scenarios complete. Closing." << endl;
             break;
        }
    }

    close(receiver_sock);
    return 0;
}