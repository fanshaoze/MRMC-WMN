#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "meshclient.h"

int shutdown_net(){
    return 0;
}




void radio_init(struct radio_type * radios){
    int i = 0;
    for (i = 0;i<radio_no;i++){
        radios[i].disabled = 0;
        strcpy(radios[i].mode,init_mode);
        radios[i].wds = init_wds;
        strcpy(radios[i].channel,init_channel_5G);
        strcpy(radios[i].ssid,init_ssid);
    }
}

int confirm_wireless(){
    system("uci commit wireless");
    system("/etc/init.d/network restart");
    return 0;
}

//changed

int alloc_config(struct radio_type radio){
    char set_channel[100];
    char set_ssid[100];
    char set_mode[100];
    char set_wds[100];
    char set_hwmode[100];
     char id_temp[10];
    char * no_in_wlan;
    snprintf(id_temp,10,"%s",radio.radio_id);
    no_in_wlan = strtok(id_temp,"h");
    printf("radio_id %s\n",radio.radio_id);
    no_in_wlan = strtok(NULL,"h");
    sprintf(set_channel,"%s%s%s%s","uci set wireless.@wifi-device[",no_in_wlan,"].channel=",radio.channel);
    sprintf(set_ssid,"%s%s%s%s","uci set wireless.@wifi-iface[",no_in_wlan,"].ssid=",radio.ssid);
    sprintf(set_mode,"%s%s%s%s","uci set wireless.@wifi-iface[",no_in_wlan,"].mode=",radio.mode);
    sprintf(set_wds,"%s%s%s%d","uci set wireless.@wifi-iface[",no_in_wlan,"].wds=",radio.wds);
    sprintf(set_hwmode,"%s%s%s%s","uci set wireless.@wifi-iface[",no_in_wlan,"].hwmode=","11ac");
    printf("set_channel %s\n",set_channel);
    printf("set_ssid %s\n",set_ssid);
    printf("set_mode %s\n",set_mode);
    printf("set_ssid %s\n",set_ssid);
    printf("set_wds %s\n",set_wds);
    printf("set_hwmode %s\n",set_hwmode);
    /*
    system(set_channel);
    system(set_ssid);
    system(set_mode);
    system(set_wds);
    system(set_hwmode);
     * */
    return 0;
}

//changed
int alloc_config_all(struct radio_type * radios){
    char set_channel[100];
    char set_ssid[100];
    char set_mode[100];
    char set_wds[100];
    char set_hwmode[100];
    char * no_in_wlan;
    char id_temp[10];
    int i = 0;
    for(i = 0;i<radio_no;i++){
        snprintf(id_temp,10,"%s",radios[i].radio_id);
        no_in_wlan = strtok(id_temp,"h");
        no_in_wlan = strtok(NULL,"h");
        //no_in_wlan.substr(4,strlen(radio.radio_id));
        sprintf(set_channel,"%s%s%s%s","uci set wireless.@wifi-device[",no_in_wlan,"].channel=",radios[i].channel);
        sprintf(set_ssid,"%s%s%s%s","uci set wireless.@wifi-iface[",no_in_wlan,"].ssid=",radios[i].ssid);
        sprintf(set_mode,"%s%s%s%s","uci set wireless.@wifi-iface[",no_in_wlan,"].mode=",radios[i].mode);
        sprintf(set_wds,"%s%s%s%d","uci set wireless.@wifi-iface[",no_in_wlan,"].wds=",radios[i].wds);
        sprintf(set_hwmode,"%s%s%s%s","uci set wireless.@wifi-iface[",no_in_wlan,"].hwmode=","11ac");
        printf("set_channel %s\n",set_channel);
        printf("set_ssid %s\n",set_ssid);
        /*
        system(set_channel);
        system(set_ssid);
        system(set_mode);
        system(set_wds);
        system(set_hwmode);
         * */
        no_in_wlan=NULL;
    }
    /* recover
    confirm_wireless();
     */
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
    char rx[20];
    char tx[20];
    char signal[10];
    char noise[10];
    printf("neighbor.rx_rate %f\n",neighbor.rx_rate);
    sprintf(rx, "%f", neighbor.rx_rate);
    sprintf(tx, "%f", neighbor.tx_rate);
    sprintf(signal, "%d", neighbor.signal);
    sprintf(noise, "%d", neighbor.noise);
	printf("     %s     \n",rx);
    char * nei_inform;
    nei_inform = (char *)malloc(strlen(neighbor.mac_addr)+1+strlen(signal)+1+strlen(noise)+1+strlen(tx) +1+strlen(neighbor.tx_qam)+1+strlen(rx)+1+strlen(neighbor.rx_qam));
	sprintf(nei_inform,"%s%s%s%s%s%s%s%s%s%s%s%s%s",neighbor.mac_addr,"#",signal,"#",noise,"#",tx,"#",neighbor.tx_qam,"#",rx,"#",neighbor.rx_qam);
    printf("nei_inform %s\n",nei_inform);
	return nei_inform;
}

//changed
int radio_disable(struct radio_type radio){
    radio.disabled = 1;
    char set_disabled[100];
    char * no_in_wlan;
     char id_temp[10];
    int i = 0;
    snprintf(id_temp,10,"%s",radio.radio_id);
    no_in_wlan = strtok(id_temp,"h");
    no_in_wlan = strtok(NULL,"h");
    sprintf(set_disabled,"%s%s%s","uci set wireless.@wifi-device[",no_in_wlan,"].disabled=1");
    printf("%s\n",set_disabled);
    system(set_disabled);
    //system(set_ssid);
    //system("uci commit wireless");
    //system("/etc/init.d/network restart");
    return 0;
}
//changed
int radio_disable_all(struct radio_type * radios){
    char set_disabled[100];
    char * no_in_wlan;
    char id_temp[10];
    
    int i = 0;
    for(i = 0;i<radio_no;i++){
        radios[i].disabled = 1;
        snprintf(id_temp,10,"%s",radios[i].radio_id);
        no_in_wlan = strtok(id_temp,"h");
        no_in_wlan = strtok(NULL,"h");
        sprintf(set_disabled,"%s%s%s","uci set wireless.@wifi-device[",no_in_wlan,"].disabled=1");
        printf("%s\n",set_disabled);
        //system(set_disabled);
    }
    confirm_wireless();
    return 0;
}


//changed
int enable_all_radios(struct radio_type * radios){
    char set_enabled[100];
    char * no_in_wlan;
    char id_temp[10];
    int i = 0;
    for(i = 0;i<radio_no;i++){
        radios[i].disabled = 1;
        snprintf(id_temp,10,"%s",radios[i].radio_id);
        no_in_wlan = strtok(id_temp,"h");
        no_in_wlan = strtok(NULL,"h");
        sprintf(set_enabled,"%s%s%s","uci set wireless.@wifi-device[",no_in_wlan,"].disabled=0");
        printf("%s\n",set_enabled);
        //system(set_disabled);
    }
    confirm_wireless();
    return 0;
}

char * tok_forward(char* words, int index,char * signal){
    int i = 0;
    for(i = 0;i<index;i++){
        words = strtok(NULL,signal);
    }
    return words;
}