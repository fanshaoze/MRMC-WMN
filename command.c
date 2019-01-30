#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "meshclient.h"

int decode_command(char recv_command[]){
    
    char * words = NULL;
    words = strtok(recv_command," ");
    printf("deconde %s\n",words);
    if(strcmp(words,"SHUTDOWN") == 0){
        return 1;
    }
    else if(strcmp(words,"SETLINK") == 0){
        return 2;
    }
    else if(strcmp(words,"DISCOVER") == 0){
        return 3;
    }
    return 0;
}

char command_no(){
    return 'd';
}

int get_channel_ssid(struct radio_type * radios,char * recv_commmand){
    int i = 0;
    int j = 0;
    char * words;
    words = strtok(recv_commmand," ");
    for(i = 1;i<radio_no;i++){
        char * splite_parameter;
        words = tok_forward(words,1," ");
        char paras[200];
        strcpy(paras,words);
        splite_parameter = strtok(paras,"#");
        for(j = 0;j<radio_no;j++){
            if(strcmp(splite_parameter,radios[j].mac_addr) == 0){
                splite_parameter = tok_forward(splite_parameter,1,"#");
                if (strcmp(splite_parameter,"disable") != 0){
                    strcpy(radios[j].channel,splite_parameter);
                    splite_parameter = tok_forward(splite_parameter,1,"#");
                    strcpy(radios[j].ssid, splite_parameter);
                }
                else{
                    radio_disable(radios[j]);
                }
            }
        }
        splite_parameter[0] = '\0';
    }
    return 0;
}