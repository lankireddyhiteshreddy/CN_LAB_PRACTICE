    #ifndef PROTOCOLS_H
    #define PROTOCOLS_H

    #include<bits/stdc++.h>
    #include<arpa/inet.h>
    #include<sys/socket.h>
    #include<netinet/in.h>

    using namespace std;

    #define PORT 8080
    #define HOST "127.0.0.1"


    typedef struct __attribute__((packed)) IPHeader{
        //row 1
        uint8_t ver_hlen;
        uint8_t service_type;
        uint16_t total_length;

        //row2
        uint16_t identification;
        uint16_t flags_fragmentation;

        //row3
        uint8_t ttl;
        uint8_t protocol;
        uint16_t checksum;

        //row4
        uint32_t source_ip;
        uint32_t dest_ip;

    }IPHeader;

    uint16_t checksumModule(IPHeader *header){
        uint8_t* ptr = (uint8_t*)header;
        int header_len = sizeof(IPHeader);

        uint32_t sum = 0;
        for(int i=0;i<header_len;i+=2){
            uint16_t word;

            memcpy(&word,&ptr[i],sizeof(uint16_t));

            sum+=ntohs(word); 
        }

        while(sum>>16){
            sum = (sum&0xFFFF) + (sum>>16);
        }
        return (uint16_t)(~sum);
    }

    #endif

