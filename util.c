#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "meshclient.h"

int shutdown_net(){
    return 0;
}



/* 功  能：将str字符串中的oldstr字符串替换为newstr字符串
 * 参  数：str：操作目标 oldstr：被替换者 newstr：替换者
 * 返回值：返回替换之后的字符串
 * 版  本： V0.2
 */
void radio_init(struct radio_type * radios){
    int i = 0;
    for (i = 0;i<radio_no;i++){
        strcpy(radios[i].channel,init_channel);
        strcpy(radios[i].ssid,init_ssid);
    }
}
int alloc_channel_ssid(struct radio_type radio){
    char set_channel[100];
    char set_ssid[100];
    char * no_in_wlan;
    no_in_wlan = strtok(radio.radio_id,"n");
    //no_in_wlan.substr(4,strlen(radio.radio_id));
    sprintf(set_channel,"%s%s%s%s","uci set wireless.@wifi-device[",no_in_wlan+1,"].channel=",radio.channel);
    sprintf(set_ssid,"%s%s%s%s","uci set wireless.@wifi-device[",no_in_wlan+1,"].ssid=",radio.ssid);
    system(set_channel);
    system(set_ssid);
    system("uci commit wireless");
    system("/etc/init.d/network restart");
    return 0;
}
char *strrpc(char *str,char *oldstr,char *newstr){
    char bstr[strlen(str)];//转换缓冲区
    memset(bstr,0,sizeof(bstr));
 
    for(int i = 0;i < strlen(str);i++){
        if(!strncmp(str+i,oldstr,strlen(oldstr))){//查找目标字符串
            strcat(bstr,newstr);
            i += strlen(oldstr) - 1;
        }else{
        	strncat(bstr,str + i,1);//保存一字节进缓冲区
	    }
    }
 
    strcpy(str,bstr);
    return str;
}

char * compose_neighbor(struct node_neighbor neighbor){
    neighbor.noise = -95;
    char rx[10];
    char tx[10];
    char signal[10];
    char noise[10];
    sprintf(rx, "%f", neighbor.rx_rate);
    sprintf(tx, "%f", neighbor.tx_rate);
    sprintf(signal, "%d", neighbor.signal);
    sprintf(noise, "%d", neighbor.noise);
    char * nei_inform;
    nei_inform = (char *)malloc(strlen(neighbor.mac_addr)+1+strlen(signal)+1+strlen(noise)+1+strlen(tx) \
    +1+strlen(neighbor.tx_qam)+1+strlen(rx)+1+strlen(neighbor.rx_qam));
    sprintf(nei_inform,"%s%s%s%s%s%s%s%s%s%s%s%s%s",neighbor.mac_addr,"#",signal,"#",noise,"#",tx \
    ,"#",neighbor.tx_qam,"#",tx,"#",neighbor.tx_qam);
    return nei_inform;
}

int radio_disable(struct radio_type radio){
    char set_disabled[100];
    char * no_in_wlan;
    no_in_wlan = strtok(radio.radio_id,"n");
    sprintf(set_disabled,"%s%s%s","uci set wireless.@wifi-device[",no_in_wlan+1,"].disabled=1");
    system(set_disabled);
    //system(set_ssid);
    system("uci commit wireless");
    system("/etc/init.d/network restart");
    return 0;
}

int enable_all_radios(struct radio_type * radios){
    int i = 0;
    char set_enabled[100];
    char * no_in_wlan;
    for(i = 0;i<radio_no;i++){
        no_in_wlan = strtok(radios[i].radio_id,"n");
        sprintf(set_enabled,"%s%s%s","uci set wireless.@wifi-device[",no_in_wlan+1,"].disabled=0");
        system(set_enabled);
        //system(set_ssid);
    }
    system("uci commit wireless");
    system("/etc/init.d/network restart");
    return 0;
}