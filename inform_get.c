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
    //本机命令行ifconfig，获取radio，判断该radio为2.4还是5，并且确定该radio的ID是wlan*
    system("sh /home/fan/getradio.sh");
    //system("sh /root/getradios.sh");
    char filename[] = "radios"; //文件名
    FILE *fp; 
    char StrLine[1024];             //每行最大读取的字符数
    int freq;
    int g_radio_num = 0;//表示2.4G射频的数量
    int ac_radio_num = 0;//表示5G射频的数量
   // int i = 0;
    int j = 0;
    //struct node_neighbor node;
    //以下数据用于的radios每个射频信息的赋值
    char radios_mac_addr[radio_max][50];//存储获取的所有射频的mac地址
    char radios_id[radio_max][20];//存储获取的射频的id  wlan0,wlan1,wlan2……
    int radios_freq[radio_max];//存储获取的所有射频的频段 2.4/5
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    { 
        printf("error!"); 
        return -1; 
    } 
    while (!feof(fp)) 
    { 
        fgets(StrLine,1024,fp);  //读取一行
        printf("%s\n", StrLine); //输出
        words = strtok(StrLine," ");//按照空格划分一行的内容
        //获取标识的前四位，如果是wlan，证明这一行往下是一个无线网卡的信息
        strncpy(wl,words,4);
        if (strcmp(wl, "wlan") == 0){
            freq = get_freq(words);//根据该行是wlan*来获取可用的信道数量，进而判断是2.4还是5
            strcpy(radios_mac_addr[j],words+4);//存储该radio的mac地址到mac数组中
            strcpy(radios_id[j],words);//存储该radio的wlan标示到数组中
            //计算5G和2.4G的数量，这里以后可能会用于区分5G和2.4G，以后只用5G组网时使用
            if(freq == 5){
                if (strcmp(words, "wlan0") == 0){
                    strcpy(node_id,words+4);//默认使用wlan0的地址作为node_id
                }
                ac_radio_num += 1;
            }
            else if(freq == 2){//如果是2.4G
                if (strcmp(words, "wlan0") == 0){
                    strcpy(node_id,words+4);
                }
                g_radio_num += 1;
            }
            radios_freq[j] = freq;//存储频段信息到数据中，
        }
        
        else continue;
    }
    fclose(fp);  
    //radio_no = ac_radio_num;
    radio_no = ac_radio_num+g_radio_num;
    //为radios分配空间
    radios = (struct radio_type*) malloc(radio_no);
    //按顺序为每个radio赋值
    for(j = 0;j<radio_no;j++){
        strcpy(radios[j].radio_id,radios_id[j]);
        strcpy(radios[j].mac_addr,radios_mac_addr[j]);
        radios[j].freq = radios_freq[j];
    }
    //为每个radio设置初始化的射频和信道，这里init没有让ssid和channel生效，只是赋值
    radio_init(radios);
    return 0;
}
int get_neighbor(int clientSocket,struct radio_type * radios){
    //使能所有的射频
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
        printf("neigh_send_inform：%s\n",neigh_send_inform);
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
        //判断是不是第一行，如果是，从第一行中获取mac地址（根据命令行输出的具体格式而定）
        if (strcmp(flag, "Station") == 0){
            station_number+=1;//累积，用于计算共有多少个邻居节点，并分配空间
            strcpy(mac_addr,words+1);
            printf("%s\n",mac_addr);
        }
    }
    radio.neighbors = (struct node_neighbor*) malloc(station_number);
    rewind(fp);//回到文件头部，重新扫描，并保存邻居信息
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
            strcpy(radio.neighbors[t].mac_addr,mac_addr);//获取mac地址
        }
        else if(strcmp(flag, "signal:") == 0){
            strcpy(signal,words+1);
            printf("%s\n",signal);
            //char * signal_splite = strtok(,"M");
            int num=atoi(signal);
            radio.neighbors[t].signal = num;//获取信号强度
        }
        else if(strcmp(flag, "tx") == 0){
            if(strcmp(words+1,"bitrate:") == 0){
                strcpy(tx_rate_str,words+2);
                printf("%s\n",tx_rate_str);
                int num=atoi(rx_rate_str);
                radio.neighbors[t].tx_rate = num;
                words = strtok(StrLine,"Bit/s ");
                strrpc(words+1," ","$");
                strcpy(radio.neighbors[t].rx_qam,words+1);//获取tx速率
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
                strcpy(radio.neighbors[t].rx_qam,words+1);//获取rx速率
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
                