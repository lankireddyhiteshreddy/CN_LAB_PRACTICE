#ifndef CRC_COMMON_H
#define CRC_COMMON_H

#include<bits/stdc++.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

using namespace std;

#define PORT 8080
#define HOST "127.0.0.1"

#define POLY "1100000001111"
#define CRC_BITS 12
#define PACKET_SIZE 100

typedef struct Packet{
    char data[PACKET_SIZE];
    uint16_t crc_checksum;
    int bytes_in_packet;
    bool is_last_packet;
}Packet;

string bytesToBinary(const char* data,int len){
    string binary_string = "";
    for(int i=0;i<len;i+=1){
        binary_string+=bitset<8>(data[i]).to_string();
    }
    return binary_string;
}

uint16_t binaryToChecksum(string remainder){
    return (uint16_t)(bitset<16>(remainder).to_ulong());
}

string crcMod2Division(string dividend){
    string polynomial = POLY;
    int dividend_length = dividend.length();
    int poly_length = polynomial.length();

    string temp_dividend = dividend.substr(0,poly_length);

    for(int i=poly_length;i<=dividend_length;i+=1){
        if(temp_dividend[0]=='1'){
            for(int j=0;j<poly_length;j+=1){
                temp_dividend[j] = (temp_dividend[j]==polynomial[j])?'0':'1';
            }
        }
        temp_dividend = temp_dividend.substr(1);

        if(i<dividend_length){
            temp_dividend+=dividend[i];
        }
    }
    return temp_dividend;
}

uint16_t calculateCRC(const char* data,int len){
    string binary_data = bytesToBinary(data,len);

    string padded_data = binary_data;
    for(int i=0;i<CRC_BITS;i+=1){
        padded_data+='0';
    }

    string remainder = crcMod2Division(padded_data);

    return binaryToChecksum(remainder);
}

string u_intToBinary(uint16_t crc_checksum){
    string binary_string = bitset<16>(crc_checksum).to_string();

    return binary_string.substr(16-CRC_BITS);
}

bool verifyCRC(Packet* packet){
    string binary_data = bytesToBinary(packet->data,packet->bytes_in_packet);
    
    string binary_crc = u_intToBinary(ntohs(packet->crc_checksum));

    string full_frame = binary_data + binary_crc;

    string remainder = crcMod2Division(full_frame);

    for(char c : remainder){
        if(c=='1'){
            return false;
        }
    }
    return true;
}

#endif