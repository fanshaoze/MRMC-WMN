#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "meshclient.h"

int get_nodeid(){
    return 0;
}
int radios_inform_init(struct radio_type * radios){
    //enable_all_radios()
    char * words;
    char * wl = "";
    system("sh /home/fan/getradio.sh");
    char filename[] = "radios"; //文件名
    FILE *fp; 
    char StrLine[1024];             //每行最大读取的字符数
    int freq;
    int g_radio_num = 0;
    int ac_radio_num = 0;
   // int i = 0;
    int j = 0;
    //struct node_neighbor node;
    char radios_mac_addr[radio_max][50];
    char radios_id[radio_max][20];
    int radios_freq[radio_max];
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    { 
        printf("error!"); 
        return -1; 
    } 
    while (!feof(fp)) 
    { 
        fgets(StrLine,1024,fp);  //读取一行
        printf("%s\n", StrLine); //输出
        words = strtok(StrLine," ");
        strncpy(wl,words,4);
        if (strcmp(wl, "wlan") == 0){
            freq = get_freq(wl);
            strcpy(radios_mac_addr[j],words+4);
            strcpy(radios_id[j],words);
            //如果是5G，获取邻居，否则continue
            if(freq == 5){
                if (strcmp(words, "wlan0") == 0){
                    strcpy(node_id,words+4);
                }
                ac_radio_num += 1;
                
                
            }
            else if(freq == 2){//如果是2.4G
                if (strcmp(words, "wlan0") == 0){
                    strcpy(node_id,words+4);
                }
                g_radio_num += 1;
            }
            radios_freq[j] = freq;
        }
        
        else continue;
    }
    fclose(fp);  
    //radio_no = ac_radio_num;
    radio_no = ac_radio_num+g_radio_num;
    radios = (struct radio_type*) malloc(radio_no);
    for(j = 0;j<radio_no;j++){
        strcpy(radios[j].radio_id,radios_id[j]);
        strcpy(radios[j].mac_addr,radios_mac_addr[j]);
        radios[j].freq = radios_freq[j];
    }
    radio_init(radios);
    return 0;
}
int get_neighbor(int clientSocket,struct radio_type * radios){
    enable_all_radios(radios);
    int i = 0;
    for(i = 0;i<radio_no;i++){
        char neigh_send_inform[10000];
        char radio_result[1000];
        get_nei_infor(radio_result,radios[i]);
        strcat(neigh_send_inform,"NEIGHBOR ");
        strcat(neigh_send_inform,node_id);
        strcat(neigh_send_inform," ");
        strcat(neigh_send_inform,radios[i].channel);
        strcat(neigh_send_inform," ");
        strcat(neigh_send_inform,radios[i].mac_addr);
        strcat(neigh_send_inform," ");
        strcat(neigh_send_inform, radio_result);
        strcat(neigh_send_inform, "\r\n");
        send_neighbor(clientSocket,neigh_send_inform);
        neigh_send_inform[0] = '\0';
        
    }
    
                       //关闭文件
    return 0; 
}





int get_freq(char * wlan){
    char filename[] = "channels"; //文件名
    char * orig_com = "sh /home/fan/gechannels.sh";
    FILE *fp; 
    char * words;
    char * channel_number = " ";
    char StrLine[1024];   
    char *sh_com = (char *) malloc(strlen(orig_com) + strlen(wlan));
    
    strcpy(sh_com, orig_com);
    strcat(sh_com, wlan);
    system(sh_com);
    
              //每行最大读取的字符数
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    { 
        printf("error!"); 
        return -1; 
    } 
    if (!feof(fp)) { 
        fgets(StrLine,1024,fp);  //读取一行
        printf("%s\n", StrLine); //输出
        words = strtok(StrLine," ");
        strcpy(channel_number,words+1);
        if (strcmp(channel_number, "11") == 0){
            return 2;
        }
        else if (strcmp(channel_number, "25") == 0){
            return 5;
        }
        else return 0;
    }
    return 0;
}

int get_nei_infor(char * result, struct radio_type radio){
    //struct node_neighbor nei_node[];
    
    //alloc_channel_ssid(radio);
    char filename[] = "neighbors"; //文件名
    char * orig_com = "sh /home/fan/shelltest.sh";
    FILE *fp; 
    char * words;
    char mac_addr[100];
    char rx_rate_str[20];
    char tx_rate_str[20];
    char signal[20];
    char StrLine[1024];   
    char flag[20];
    int station_number = 0;
    
              //每行最大读取的字符数
    system(orig_com);
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    { 
        printf("error!"); 
        return -1; 
    } 
    while (!feof(fp)) { 
        fgets(StrLine,1024,fp);  //读取一行
        printf("%s\n", StrLine); //输出
        words = strtok(StrLine," ");
        strcpy(flag,words);
        if (strcmp(flag, "Station") == 0){
            station_number+=1;
            strcpy(mac_addr,words+1);
            printf("%s\n",mac_addr);
        }
    }
    radio.neighbors = (struct node_neighbor*) malloc(station_number);
    rewind(fp);
    int t =0;
    while (!feof(fp)) { 
        fgets(StrLine,1024,fp);  //读取一行
        printf("%s\n", StrLine); //输出
        words = strtok(StrLine," ");
        strcpy(flag,words);
        if (strcmp(flag, "Station") == 0){
            station_number+=1;
            strcpy(mac_addr,words+1);
            printf("%s\n",mac_addr);
        }
        else if(strcmp(flag, "signal:") == 0){
            strcpy(signal,words+1);
            printf("%s\n",signal);
            //char * signal_splite = strtok(,"M");
            int num=atoi(signal);
            radio.neighbors[t].signal = num;
        }
        else if(strcmp(flag, "tx") == 0){
            if(strcmp(words+1,"bitrate:") == 0){
                strcpy(tx_rate_str,words+2);
                printf("%s\n",tx_rate_str);
                int num=atoi(rx_rate_str);
                radio.neighbors[t].tx_rate = num;
                words = strtok(StrLine,"Bit/s ");
                strrpc(words+1," ","$");
                strcpy(radio.neighbors[t].rx_qam,words+1);
            }
        }
        else if(strcmp(flag, "rx") == 0){
            if(strcmp(words+1,"bitrate:") == 0){
                strcpy(rx_rate_str,words+2);
                printf("%s\n",rx_rate_str);
                int num=atoi(rx_rate_str);
                radio.neighbors[t].rx_rate = num;
                words = strtok(StrLine,"Bit/s ");
                strrpc(words+1," ","$");
                strcpy(radio.neighbors[t].rx_qam,words+1);
            }
        }
        else if(strcmp(flag, "authorized:") == 0){
            //radio.neighbors[t];
            strcat(result," ");
            strcat(result,compose_neighbor(radio.neighbors[t]));
            t++;
        }
    }
    return 0;
}

//int alloc_channel_ssid(struct radio_type *radios){
    //uci set wireless.@wifi-device[0].channel=6    //设置无线信道为6  
                //uci set wireless.@wifi-iface[0].mode=ap    //设置无线模式为ap  
                //uci set wireless.@wifi-iface[0].ssid=[自己设置SSID]    //设置无线SSID 
    
    
//}
                