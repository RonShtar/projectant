
#include "user.h" //include user functions
int main(int args, char *argv[])
{
    init_socket();  //init socket
    transfer_data(); //transfer data with kernel space
    return 0;
}
