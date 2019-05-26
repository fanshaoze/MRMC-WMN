#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
/*socket tcp客户端*/
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define init_channel_2G "11"
#define init_ssid_2G "Link0"
#define init_channel_5G "36"
#define init_ssid_5G "Link1"
#define init_mode "ap"
#define init_wds 0

#define radio_max 20
#define hdaddr_len 18

#define freqcom "iwlist "
#define freqfile " "
#define wlancom "ifconfig"
#define wlanfile "/home/fan/codelite/mesh-client/ifconfig"
#define neicom0 "iw dev "
#define neicom1 " station dump"

#define sendport 10001
#define receiveport 10002



char node_id[50];
int radio_no;
int enable_time;
int findnei_time;
char init_ssid[10];
char ipaddr[20];

struct radio_type{
    char radio_id[10];
    char mac_addr[50];
    char channel[10];
    char ssid[10];
    char mode[10];
    int neigh_count;
    int freq;
    int wds;
    struct node_neighbor * neighbors;
    int disabled;
};

struct node_neighbor{
    char mac_addr[50];
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
int decode_command(char recv_command[]);
int alloc_config(struct radio_type radio);
int alloc_config_all(struct radio_type * radios);
int get_neighbor(int clientSocket,struct radio_type * radios);
int send_neighbor(int clientSocket, char * neigh_inform);
int shutdown_net();
int get_freq(char * wlan);
char * get_nei_infor(char * result, struct radio_type radio);
int get_config(struct radio_type * radios,char * recv_commmand);
int radio_disable(struct radio_type radio);
int radio_disable_all(struct radio_type * radios);
int enable_all_radios(struct radio_type * radios);
int radios_inform_init();
void radios_inform_init2(struct radio_type * radios);

char *strrpc(char *str,char *oldstr,char *newstr);
void radio_init(struct radio_type * radios);
char * compose_neighbor(struct node_neighbor neighbor);
int confirm_wireless();
char * tok_forward(char* words, int index,char * signal);