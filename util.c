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
void radio_init(struct radio_type radio){
    strcpy(radio.channel,init_channel);
    strcpy(radio.ssid,init_ssid);
}
int alloc_channel_ssid(struct radio_type radio){
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