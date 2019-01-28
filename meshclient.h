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


#define init_channel "36"
#define init_ssid "mesh"
#define radio_max 10

char node_id[50];
int radio_no;

struct radio_type{
    char radio_id[10];
    char mac_addr[100];
    char channel[10];
    char ssid[10];
    int neigh_count;
    int freq;
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
int decode_command(char * recv_command);
int alloc_channel_ssid(struct radio_type radio);
int get_neighbor(int clientSocket,struct radio_type * radios);
int send_neighbor(int clientSocket, char * neigh_inform);
int shutdown_net();
int get_freq(char * wlan);
int get_nei_infor(char * result, struct radio_type radio);
int get_channel_ssid(struct radio_type * radios,char * recv_commmand);
int radio_disable(struct radio_type radio);
int enable_all_radios(struct radio_type * radios);
int radios_inform_init(struct radio_type * radios);

char *strrpc(char *str,char *oldstr,char *newstr);
void radio_init(struct radio_type * radios);
char * compose_neighbor(struct node_neighbor neighbor);