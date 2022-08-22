#ifndef USER_H
#define USER_H
#define PORT 30 // same customized protocol as in my kernel module
#define MAX_PAYLOAD 3 // maximum payload size
#define MAX_BUFFER 20 // max size of buffer that is sent in packet
int init_socket(); //initialize net link to communicate with kernel
int transfer_data(); //transfer data
#endif