#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "meshclient.h"

int get_nodeid(){
    return 0;
}
int get_neighbor(struct radio_type * radios){
    char * words;
    char * wl = "";
    system("sh /home/fan/getradio.sh");
    char filename[] = "radios"; //文件名
    FILE *fp; 
    char StrLine[1024];             //每行最大读取的字符数
    int freq;
    int g_radio_num = 0;
    int ac_radio_num = 0;
    int i = 0;
    //struct node_neighbor node;
    
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
        strcpy(wl,words);
        if (strcmp(wl, "wlan") == 0){
            freq = get_freq(wl);
            //如果是5G，获取邻居，否则continue
            if(freq == 5){
                
                ac_radio_num += 1;
            }
            else if(freq == 2){//如果是2.4G
                g_radio_num += 1;
            }
            
        }
        else continue;
        
    }
    fclose(fp);  
    radios = (struct radio_type*) malloc(ac_radio_num);
    for(i = 0;i<ac_radio_num;i++){
        get_nei_infor(radios[i]);
    }
    
                       //关闭文件
    return 0; 
}


int alloc_channel_ssid(){
    return 0;
}


int get_freq(char * wlan){
    char filename[] = "channels"; //文件名
    char * orig_com = "sh /home/fan/gechannels.sh";
    FILE *fp; 
    char * words;
    char * channel_number = "";
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

int get_nei_infor(struct radio_type radio){
    //struct node_neighbor nei_node[];
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
    radio_init(radio,station_number);
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
            //compose_neighbor();
            t++;
            }
       }
    return 0;
}