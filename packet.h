#define mac_bytes 6
#define ip_bytes 4
#define content_bytes 4
struct ethHead //ethernet header struct contains 6 bytes of source mac address and destination mac address,2 bytes of ether type
{
    // long s_mac:48;
    unsigned char d_mac[mac_bytes]; //destination physical address 
    unsigned char s_mac[mac_bytes]; //source physical address
    __u16 packet_type;  //type of information 0800-ipv4 0806-arp
};
struct ipHead
{
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        __u8 ihl : 4; //version of packet,4 means ipv4
        __u8 version : 4; //header length in dwords
    #elif __BYTE_ORDER == __BIG_ENDIAN
        __u8 version : 4; //version of packet,4 means ipv4
        __u8 ihl : 4;     //header length in dwords
    #endif
    __u8 tos; //determine preferences speed/authenticity
    __u16 totalLen; //packet length without eth header len
    __u16 id;  //id of packet in order to know which packet are related to each other(splitted packets)
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        __u16 fragoff : 12;  //check if the packet should be spiltted mtu/pdu
        __u8 flags : 4; //flags that contain information about splitted packets that more than one
    #elif __BYTE_ORDER == __BIG_ENDIAN
        __u8 flags : 4;  //flags that contain information about splitted packets that more than one
        __u16 fragoff : 12;  //check if the packet should be spiltted mtu/pdu
    #endif
    __u8 ttl;       //time to live - how many hops between routers remains
    __u8 protocol;  //protocol of packet udp=17 tcp=6 icmp=1
    __u16 checksum; //checksum field to make sure that data is correct (of header)
    unsigned char s_addr[ip_bytes]; //sender ip 
    unsigned char d_addr[ip_bytes]; //reciever ip
};
struct tcpHead
{
    __u16 s_port; //source port
    __u16 d_port; //destination port
    __u32 seq; //sequence number is depends on syn flag syn=1 -> the new number that establish the connection with server syn=0 -> accumulated sequence number of the first data byte 
    __u32 ack_num; //acknowledge number that confirms meassages from the client that recievd 
    #if __BYTE_ORDER == __LITTLE_ENDIAN
	    __u16 
            reserved:4, //future using
            data_off:4; //header size in dwords
        __u8  
             fin:1,
             syn:1,
             rst:1,
             psh:1,
             ack:1,
             urg:1,
             ece:1,
             cwr:1;    
    #elif __BYTE_ORDER == __BIG_ENDIAN
        __u16 data_off:4,
               reserved:4;
        __u8 cwr:1,
             ece:1,
             urg:1,
             ack:1,
             psh:1,
             rst:1,
             syn:1, //establish the connection 
             fin:1; //end of connection
    #endif
	__u16 window; //the number of bytes that the sender of this segment is currently willing to receive
	__u16 checksum; //checksum field to make sure that data is correct (of header)
	__u16 urgentPointer; //for important meassage
    /*#if __BYTE_ORDER == __LITTLE_ENDIAN
	    __u16 
            reserved:6,
            data_off:4;
        
        __u8  
             fin:1,
             syn:1,
             rst:1,
             psh:1,
             ack:1,
             urg:1;
             
    #elif __BYTE_ORDER == __BIG_ENDIAN
        __u16 data_off:4,
               reserved:6;
        __u8 urg:1,
             ack:1,
             psh:1,
             rst:1,
             syn:1,
             fin:1;
    #endif*/
};
struct udpHead
{
    __u16 s_port; //source port 
    __u16 d_port; //destination port
    __u16 length; //packet length without eth header and ip header length
    __u16 checksum; //checksum field to make sure that data is correct (of header)
};
struct icmpHead
{
    __u8 type;      //type of message response=0  request=8 tracert=30
    __u8 code;      //more details about the message port is unreachable=3
    __u16 checksum; //checksum field to make sure that data is correct
    __u32 content; //data that points out the problem in the original in ip message
};
