#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct radio_type{
    char mac_addr[100];
    int neigh_count;
    struct node_neighbor * neighbors;
};

struct node_neighbor{
    char mac_addr[100];
    int signal;
    int noise;
    float rx_rate;
    char rx_qam[50]; 
    float tx_rate;
    char tx_qam[50]; 
    
};
struct command{
    
};

int get_nodeid();
int decode_command();
int alloc_channel_ssid();
int get_neighbor();
int send_neighbor();
int shutdown_net();
char command_no();
int get_freq(char * wlan);
int get_nei_infor(struct radio_type radio);

char *strrpc(char *str,char *oldstr,char *newstr);
void radio_init(struct radio_type radio,int neighbor_num);