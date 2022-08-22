#ifndef PACKETSEND_H
#define PACKETSEND_H
#define DESTMAC0 0x00  //destination mac
#define DESTMAC1 0xe0
#define DESTMAC2 0x4c
#define DESTMAC3 0x82
#define DESTMAC4 0x48
#define DESTMAC5 0x2c
#define destination_ip 192.168.227.34 //destination ip address 
void printpacket(char* sendbuff); //prints packet fields
void get_eth_index(); //function that gives network interface index
char* get_mac(char* sendbuff);//function that gives mac address and make data link header
char* get_ip(char *dataToSend,char* sendbuff); //function that gives ip address and make ip header
char* get_udp(char *dataToSend,char* sendbuff); //function that make udp header
char* get_data(char* dataToSend,char* sendbuff); //function that puts on payload keystrokes
unsigned short checksum(unsigned short* buff, int _16bitword); //function that calculates checksum
int init_rawsocket(); //initialize raw socket
void build_packet(char* dataToSend,char* sendbuff); //build packet to send
int sendPacket(char* data); 

#endif