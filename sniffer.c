#include <sys/socket.h> //create socket
#include <netinet/in.h> //socket struct
#include <arpa/inet.h>  //htons htonl
#include <net/ethernet.h> //listenting packets from data link layer
#include <stdio.h>        //input and output
#include <stdlib.h>       //malloc 
#include <string.h>       //memset
#include "packet.h"  //headers sturcts

int sock_r, data_size,saddr_len;
struct sockaddr saddr;

static int initSocket()
{
    saddr_len = sizeof(saddr);
    sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // this socket receives all incoming packets of all protocols
    if (sock_r < 0) // erorr in creating this socket
    {
        printf("error in socket\n");
        return -1;
    }
    return 1;
}
static void printEth(struct ethHead *eth_p) //print ethernet header fields
{
    printf("\nEthernet Header\n");
    printf("\t|-Source Address :%.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth_p->s_mac[0], eth_p->s_mac[1], eth_p->s_mac[2], eth_p->s_mac[3], eth_p->s_mac[4], eth_p->s_mac[5]);     // print mac address in host form,printing mac address in hex base with fields of 2 width
    printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n", eth_p->d_mac[0], eth_p->d_mac[1], eth_p->d_mac[2], eth_p->d_mac[3], eth_p->d_mac[4], eth_p->d_mac[5]); // print mac address in host form,printing mac address in hex base with fields of 2 width
    printf("\t|-Packet Type : %x\n", ntohs(eth_p->packet_type)); //print packet type in host form
}
static void printIp(struct ipHead *ip_p) //print ip header fields
{
    printf("\nIP Header\n");
    printf("\t|-Version %d\n", ip_p->version);
    printf("\t|-Size of header in dwords %d and bytes %d\n", ip_p->ihl, ip_p->ihl * 4); //ihl-size of header in dword thus to display it in bytes it should be 4*ihl
    printf("\t|-Type Of Service : %d\n", ip_p->tos);               
    printf("\t|-Total Length : %d Bytes\n", ntohs(ip_p->totalLen)); // print ipv4 header+payload length in host form
    printf("\t|-Identification : %d\n", ntohs(ip_p->id));
    printf("\t|-Time To Live : %d\n", ip_p->ttl); 
    printf("\t|-Protocol : %d\n", ip_p->protocol); 
    printf("\t|-Header Checksum : %d\n", ntohs(ip_p->checksum));
    printf("\t|-Source IP : %d.%d.%d.%d\n", ip_p->s_addr[0], ip_p->s_addr[1], ip_p->s_addr[2], ip_p->s_addr[3]);
    printf("\t|-Destination IP : %d.%d.%d.%d\n", ip_p->d_addr[0], ip_p->d_addr[1], ip_p->d_addr[2], ip_p->d_addr[3]);
}
static void printTcp(struct tcpHead *tcp_p) //print tcp header fields
{
    printf("\nTCP Header\n");
    printf("\t|- Source Port : %d\n", ntohs(tcp_p->s_port));  //print source port in host form
    printf("\t|- Destination Port : %d\n", ntohs(tcp_p->d_port)); //print destination port in host form
    printf("\t|- Sequence Number : %d\n", ntohs(tcp_p->seq)); //print sequence number in host form
    printf("\t|- Acknowledge Number : %d\n", ntohs(tcp_p->ack_num)); //print ack number in host form
    printf("\t |-Header Length : %d DWORDS or %d BYTES\n", tcp_p->data_off, (tcp_p->data_off * 4));  //print header length in dwords and bytes
    printf("----------------FLAGS----------------\n");  
    printf("\t\t |-Urgent flag : %d\n", tcp_p->urg);
    printf("\t\t |-Acknowledgement flag : %d\n", tcp_p->ack);
    printf("\t\t |-push flag : %d\n", tcp_p->psh);
    printf("\t\t |-Reset flag : %d\n",tcp_p->rst);
    printf("\t\t |-Synchronise flag : %d\n", tcp_p->syn);
    printf("\t\t |-Finish flag : %d\n", tcp_p->fin);
    printf("\t\t |-Window size  :%d\n", ntohs(tcp_p->window)); 
    printf("\t\t |- checksum  :%d\n", ntohs(tcp_p->checksum));
    printf("\t\t |- Urgent pointer  :%d\n", ntohs(tcp_p->urgentPointer));
}
static void printUdp(struct udpHead *udp_p) // print udp header fields
{
    printf("\nUDP Header\n");
    printf("\t|-Source Port : %d\n", ntohs(udp_p->s_port));      // print soucre port in host form
    printf("\t|-Destination Port : %d\n", ntohs(udp_p->d_port)); // print destination port in host form
    printf("\t|-UDP Length : %d\n", ntohs(udp_p->length));       // print length of packet without ip+eth size in host form
    printf("\t|-UDP Checksum : %d\n", ntohs(udp_p->checksum));   
}
static void printIcmp(struct icmpHead *icmp_p) //print icmp header fields
{
    printf("\nICMP Header\n");
    printf("\t|-Type : %d\n", icmp_p->type); 
    printf("\t|-Code : %d\n", icmp_p->code); 
    printf("\t|-Checksum : %d\n", ntohs(icmp_p->checksum)); //print checksum in host form
    printf("\t|-Data : %d\n", (unsigned)icmp_p->content);
}
static void printData(unsigned char *data,unsigned char *buffer,int data_size) //print data
{
    int count16=0;
    printf("\nDATA:\n");
    while(data<(buffer+data_size))
    {
        if(!(count16 % 16) && count16) //new line when 16 numbers are printed 
            printf("\n");
        printf("%.2X ",*data); //print data 2 hex numbers
        data++;
        count16++;
    }
    printf("\n----------------END OF PACKET----------------\n");
}
static int sniffPackets()
{
    
    printf("socket intialized ready to catch packets...\n");
    unsigned char *buffer = (unsigned char *)malloc(65536); // make a buffer to get the packet max size of packet = 65535 bytes
    if (buffer == NULL) // error in memory allocation
    {
        return -1;
    }
    // Receive a network packet and copy in to buffer
    while (1)
    {
        memset(buffer, 0, 65536);  //clean last packet in order to get new one                                                       
        data_size = recvfrom(sock_r, buffer, 65536, 0, &saddr, (socklen_t *)&saddr_len); //receive a packet to buffer
        if (data_size < 0) 
        {
            printf("error in receiving data");
            return -1;
        }
        else
        {
            struct ethHead *eth_p = (struct ethHead *)(buffer); // make a ethernet header struct
            if (ntohs(eth_p->packet_type) == 2048) //packet type is ipv4
            {
                struct ipHead *ip_p = (struct ipHead *)(buffer + sizeof(struct ethHead)); // make a ip header struct by addding eth header size               
                if (ip_p->protocol == 6) //tcp protocol
                {
                    printf("\n******************TCP PACKET******************");
                    printEth(eth_p); //function that prints eth header
                    printIp(ip_p); //function that prints ip header
                    struct tcpHead *tcp_p = (struct tcpHead*)(buffer+sizeof(struct ethHead)+ip_p->ihl*4); // get tcp struct by adding eth header size and ip header size (dwords*4)
                    printTcp(tcp_p); //function that prints tcp header
                    unsigned char *data = (buffer + ip_p->ihl * 4 + sizeof(struct ethHead) + sizeof(struct tcpHead)); //get data part by adding eth,ip,tcp headers size
                    printData(data,buffer,data_size); //function that prints data
                }
                else if (ip_p->protocol == 17) //udp protocol
                {
                    printf("\n******************UDP PACKET******************");
                    printEth(eth_p); //function that prints eth header
                    printIp(ip_p); //function that prints ip header
                    struct udpHead *udp_p = (struct udpHead *)(buffer + sizeof(struct ethHead) + ip_p->ihl * 4); // get udp struct by adding eth header size and ip header size (dwords*4)
                    printUdp(udp_p); //function that prints udp header
                    unsigned char *data = (buffer + ip_p->ihl * 4 + sizeof(struct ethHead) + sizeof(struct udpHead)); //get data part by adding eth,ip,udp headers size
                    printData(data,buffer,data_size);  ///fucntion that prints data
                }
                else if (ip_p->protocol == 1) //icmp protocol
                {
                    printf("\n******************ICMP PACKET******************");
                    printEth(eth_p); //function that prints eth header
                    printIp(ip_p); //function that prints ip header printEth(eth_p);
                    struct icmpHead *icmp_p = (struct icmpHead *)(buffer + sizeof(struct ethHead) + ip_p->ihl * 4); // get icmp struct by adding eth header size and ip header size (dwords*4)
                    printIcmp(icmp_p);  //function that prints icmp header
                    unsigned char *data = (buffer + ip_p->ihl * 4 + sizeof(struct ethHead) + sizeof(struct icmpHead)); //get data part by adding eth,ip,icmp headers size
                    printData(data,buffer,data_size); //function that prints data
                }
            }
        }
    }
    close(sock_r);
    free(buffer);
    return 0;
}
int main()
{
    
    if(initSocket())  //return value 1 means that socket has created
    {
        sniffPackets();  // call sniffing packet fucntion
    }
    return 0;
}