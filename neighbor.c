#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "meshclient.h"

char * get_nei_infor_iwlist(char * result, struct radio_type radio){
    printf("radio.radio_id %s\n",radio.radio_id);
    //struct node_neighbor nei_node[];
    char command[100];
    sprintf(command,"%s%s%s%s%s","iwlist ",radio.radio_id," scanning > /tmp/",radio.radio_id,"scanning");
    char filename[100];
    
     system(command);
     sprintf(filename,"%s%s%s","/tmp/",radio.radio_id,"scanning");
     printf("get %s\n",filename);
/*
    if(strcmp(radio.radio_id,"wlan0") == 0)
        strcpy(filename,"/home/fan/codelite/mesh-client/dump"); //文件名
    if(strcmp(radio.radio_id,"wlan1") == 0)
        strcpy(filename,"/home/fan/codelite/mesh-client/dump"); //文件名
    filename[strlen("/home/fan/codelite/mesh-client/dump")] = '\0';
    */
    FILE *fp;
    char *words;
    char mac_addr[50];
    char signal[20];

    char StrLine[1024];
    char flag[25];
    int station_number = 0;
    char * words_next;
    char words_temp[200];

    int i = 0;
    int recordflag = 0;//用于标识“以下信噪比等信息是否需要记录”
    sleep(findnei_time);
    //************system(orig_com);
	
    fp = fopen(filename,"r");
	if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return "";
    }
    while (!feof(fp)) {
        fgets(StrLine,1024,fp);  //读取一行
        char * words_next;
        words_next = strstr(StrLine,"ESSID");
        snprintf(words_temp , 20 , "%s","ESSID:\"");
        strcat(words_temp , radio.ssid);
        strcat(words_temp , "\"");
        char temp2[20];
        
        snprintf(temp2 , 20 , "%s",StrLine);
        strrpc(temp2,"\n","");
        if(strcmp(temp2,words_temp) == 0){
            continue;
        }
        else{
            if(words_next != NULL){
                snprintf(flag,5,"%s",words_next+7);
                printf("link %s \n",flag);
                if (strcmp(flag, "Link") == 0){
                    station_number+=1;//累积，用于计算共有多少个邻居节点，并分配空间
                    //words = NULL;
                }
            }
        }
    }
    printf("station number %d\n",station_number);
    radio.neighbors = (struct node_neighbor*) malloc(sizeof(struct node_neighbor)*station_number);
    int t =0;
    snprintf(words_temp,2,"%s","");
    
    printf("station number %d\n",station_number);
    radio.neighbors = (struct node_neighbor*) malloc(sizeof(struct node_neighbor)*station_number);
   //需要修改，所有邻居信息的初始化
    for(i = 0;i<station_number;i++){
        snprintf(radio.neighbors[i].mac_addr,50,"%s","NULL");
        snprintf(radio.neighbors[i].rx_qam,50,"%s","ORIGIN");
        snprintf(radio.neighbors[i].tx_qam,50,"%s","ORIGIN");
        radio.neighbors[i].signal = 0;
        radio.neighbors[i].noise = 0;
        radio.neighbors[i].rx_rate = 0.0;
        radio.neighbors[i].tx_rate = 0.0;
    }
//    rewind(fp);//回到文件头部，重新扫描，并保存邻居信息
    if(fclose(fp)) printf("file close error!\n");
    fp = NULL;
    //fp = fopen(filename,"r");
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return "";
    }
    while (!feof(fp)) {
       fgets(StrLine,1024,fp);  //读取一行
       char str_tmp[1024];
       snprintf(str_tmp,1024,"%s",StrLine);
       words = strtok(StrLine," ");
       snprintf(flag,5,"%s",words);
       strrpc(flag,"\t","");
       strrpc(flag,"\r","");
       strrpc(flag,"\n","");
        //printf("get2 flag:%s\n",flag);
       if (strcmp(flag, "Cell") == 0){
            words = tok_forward(words,4," ");
            snprintf(mac_addr,50,"%s",words);
            printf("get current mac_addr %s\n",mac_addr);
           
        }
        else if(strcmp(flag, "ESSI") == 0){
            snprintf(flag,5,"%s",words+7);
            if (strcmp(flag, "Link") == 0){
                snprintf(words_temp , 20 , "%s","ESSID:\"");
                strcat(words_temp , radio.ssid);
                strcat(words_temp , "\"");
                char temp2[20];
                
                snprintf(temp2 , 20 , "%s",words);
                strrpc(temp2,"\n","");
		printf("ownssid %s , scanssid %s\n",words_temp,temp2);
                if(strcmp(temp2,words_temp) == 0){
                    continue;
                }
                else{
                    snprintf(radio.neighbors[t].mac_addr,50,"%s",mac_addr);
                    recordflag = 1;
                }
            }
        }
        else if(recordflag == 1 && strcmp(flag, "Qual") == 0){
            words = tok_forward(words,2," ");
            //strcpy(signal,words);
            words_next = strstr(words,"-");
            snprintf(signal,strlen(words_next)+1,"%s",words_next);
            //char * signal_splite = strtok(,"M");
            int sginalnum=atoi(signal);
            radio.neighbors[t].signal = sginalnum;//获取信号强度
            if(sginalnum < -60) {
                radio.neighbors[t].rx_rate = 520.0;
                radio.neighbors[t].tx_rate = 520.0;
            }
            else if(sginalnum >= -60 && sginalnum<0){
                radio.neighbors[t].rx_rate = 866.0;
                radio.neighbors[t].tx_rate = 866.0;
            }
            radio.neighbors[t].noise = -95;
            printf("get signal,noise %d %d\n",radio.neighbors[t].signal,radio.neighbors[t].noise);
        }
        else if(recordflag == 1 && strcmp(flag, "Encr") == 0){
            strcat(result,compose_neighbor(radio.neighbors[t]));
            strcat(result," ");
            t++;
            recordflag = 0;
        }
        else continue;        
    }
    strrpc(result,"\r","");
    strrpc(result,"\n","");
    //result[strlen(result)-1] = '\0';
	if(fclose(fp)) printf("file close error!\n");
    
    for(i = 0;i<t;i++){
        printf("neighbor:%s ,%d ,%s\n ,%f ,%d ,%s ,%f \n",\
        radio.neighbors[i].mac_addr,radio.neighbors[i].noise,radio.neighbors[i].rx_qam, \
        radio.neighbors[i].rx_rate,radio.neighbors[i].signal,radio.neighbors[i].tx_qam, \
        radio.neighbors[i].tx_rate);
        printf("************************\n");
    }
    //循环打印！！！
    printf("result %s\n",result);
    
    return result;
}

char * get_nei_infor_iw(char * result, struct radio_type radio){
    printf("radio.radio_id %s\n",radio.radio_id);
    //struct node_neighbor nei_node[];
    char command[100];
    sprintf(command,"%s%s%s%s%s","iw dev ",radio.radio_id," station dump > /tmp/",radio.radio_id,"dump");
    char filename[100];
    
     system(command);
     sprintf(filename,"%s%s%s","/tmp/",radio.radio_id,"dump");
     printf("get %s\n",filename);
/*
    if(strcmp(radio.radio_id,"wlan0") == 0)
        strcpy(filename,"/home/fan/codelite/mesh-client/dump"); //文件名
    if(strcmp(radio.radio_id,"wlan1") == 0)
        strcpy(filename,"/home/fan/codelite/mesh-client/dump"); //文件名
    filename[strlen("/home/fan/codelite/mesh-client/dump")] = '\0';
    */
    FILE *fp;
    char *words;
    char word[200];
    char mac_addr[50];
    char rx_rate_str[20];
    char tx_rate_str[20];
    char signal[20];
    char StrLine[1024];
    char flag[25];
    int station_number = 0;
    int i = 0;

    //************system(orig_com);
	
    fp = fopen(filename,"r");
	if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return "";
    }
    while (!feof(fp)) {
        fgets(StrLine,1024,fp);  //读取一行
        words = strtok(StrLine," ");
        snprintf(flag,25,"%s",words);
        //strncpy(flag,words,strlen(words));
        printf("get flag %s\n",flag);
        if (strcmp(flag, "Station") == 0){
            words = tok_forward(words,1," ");
            station_number+=1;//累积，用于计算共有多少个邻居节点，并分配空间
            snprintf(mac_addr,50,"%s",words);
            //strcpy(mac_addr,words);
            words = NULL;

        }
    }
    printf("station number %d\n",station_number);
    radio.neighbors = (struct node_neighbor*) malloc(sizeof(struct node_neighbor)*station_number);
    for(i = 0;i<station_number;i++){
        snprintf(radio.neighbors[i].mac_addr,50,"%s","NULL");
        snprintf(radio.neighbors[i].rx_qam,50,"%s","NULL");
        snprintf(radio.neighbors[i].tx_qam,50,"%s","NULL");
        radio.neighbors[i].signal = 0;
        radio.neighbors[i].noise = 0;
        radio.neighbors[i].rx_rate = 0.0;
    }
//    rewind(fp);//回到文件头部，重新扫描，并保存邻居信息
    if(fclose(fp)) printf("file close error!\n");
    fp = fopen(filename,"r");
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return "";
    }
    int t =0;
    while (!feof(fp)) {
        fgets(StrLine,1024,fp);  //读取一行
		char str_tmp[1024];
        snprintf(str_tmp,1024,"%s",words);
		//strcpy(str_tmp,StrLine);
        words = strtok(StrLine," ");
        snprintf(flag,25,"%s",words);
        //strcpy(flag,words);
	//printf("get2 flag:%s&\n",flag);
	   strrpc(flag,"\t","");
	   strrpc(flag,"\r","");
	   strrpc(flag,"\n","");
        printf("get2 flag:%s\n",flag);
	   if (strcmp(flag, "Station") == 0){
           // station_number+=1;
			words = tok_forward(words,1," ");
            snprintf(mac_addr,50,"%s",words);
            //strcpy(mac_addr,words);
            //words = tok_forward(words,1," ");
            snprintf(radio.neighbors[t].mac_addr,50,"%s",mac_addr);
            //strcpy(radio.neighbors[t].mac_addr,mac_addr);//获取mac地址
            printf("get radio.neighbors[t].mac_addr %s\n",radio.neighbors[t].mac_addr);
           
        }
        else if(strcmp(flag, "signal:") == 0){
            words = tok_forward(words,1," ");
            //strcpy(signal,words);
            snprintf(signal,strlen(words),"%s",words);
            //char * signal_splite = strtok(,"M");
            int num=atoi(signal);
            radio.neighbors[t].signal = num;//获取信号强度
			radio.neighbors[t].noise = -95;
            printf("get signal,noise %d %d\n",radio.neighbors[t].signal,radio.neighbors[t].noise);
        }
        else if(strcmp(flag, "tx") == 0){
            words = tok_forward(words,1," ");
            if(strlen(words)>=9){ 
                snprintf(word,9,"%s",words);
	    	//strncpy(word,words,8);
	    	//word[8]='\0';
            }
            else snprintf(word,9,"%s","out");
            if(strcmp(word,"bitrate:") == 0){
		        snprintf(word,200,"%s",words);
                printf("word?? %s\n",word);
		//strrpc(word,"\t","");
		//word = tok_forward(word,1,":");
                strncpy(tx_rate_str,word+9,strlen(word)-9);
                tx_rate_str[strlen(word)-9] = '\0';
		//strrpc(tx_rate_str,"\t","");
                int num=atoi(tx_rate_str);
                radio.neighbors[t].tx_rate = (float)num;
                printf("get radio.neighbors[t].tx_rate %f\n",radio.neighbors[t].tx_rate);
                char * words_next;
               // char tx_qam_str[100] = "\0";
                words_next = strstr(str_tmp,"MBit/s\n");
                printf("words_next %s\n",words_next);
                if(strcmp(words_next,"MBit/s\n") == 0){
                    snprintf(radio.neighbors[t].tx_qam,50,"%s","ORIGIN");
			//strcpy(radio.neighbors[t].tx_qam,"ORIGIN");	
                }
                else{
                    snprintf(radio.neighbors[t].tx_qam,50,"%s","ORIGIN");
            /*
			words_next = strstr(str_tmp,"MBit/s ");
                	strncpy(tx_qam_str,words_next+7,strlen(words_next)-8);
			strrpc(tx_qam_str," ","$");
                	strcpy(radio.neighbors[t].tx_qam,tx_qam_str);//获取tx速率
                    */
                }
                printf("get radio.neighbors[t].tx_qam %s\n",radio.neighbors[t].tx_qam);
            }
        }

        else if(strcmp(flag, "rx") == 0){
            words = tok_forward(words,1," ");
            if(strlen(words)>=9){ 
                snprintf(word,9,"%s",words);
            //strncpy(word,words,8);
            //word[8]='\0';
            }
            else snprintf(word,9,"%s","out");
           if(strcmp(word,"bitrate:") == 0){
                snprintf(word,200,"%s",words);
                printf("word?? %s\n",word);
        //strrpc(word,"\t","");
        //word = tok_forward(word,1,":");
                strncpy(rx_rate_str,word+9,strlen(word)-9);
                rx_rate_str[strlen(word)-9] = '\0';
        //strrpc(tx_rate_str,"\t","");
                int num=atoi(rx_rate_str);
                radio.neighbors[t].rx_rate = (float)num;
                printf("get radio.neighbors[t].rx_rate %f\n",radio.neighbors[t].rx_rate);
                char * words_next;
               // char rx_qam_str[100] = "\0";
                words_next = strstr(str_tmp,"MBit/s\n");
                printf("words_next %s\n",words_next);
        if(strcmp(words_next,"MBit/s\n") == 0){
            snprintf(radio.neighbors[t].rx_qam,50,"%s","ORIGIN");
            //strcpy(radio.neighbors[t].tx_qam,"ORIGIN");   
        }
        else{
            snprintf(radio.neighbors[t].rx_qam,50,"%s","ORIGIN");
            /*
            words_next = strstr(str_tmp,"MBit/s ");
                    strncpy(tx_qam_str,words_next+7,strlen(words_next)-8);
            strrpc(tx_qam_str," ","$");
                    strcpy(radio.neighbors[t].tx_qam,tx_qam_str);//获取tx速率
                    */
                }
        printf("get radio.neighbors[t].rx_qam %s\n",radio.neighbors[t].rx_qam);
            }
        }

        else if(strcmp(flag, "authorized:yes") == 0){
            //radio.neighbors[t];
            
            strcat(result,compose_neighbor(radio.neighbors[t]));
            strcat(result," ");
			t++;
        }
    }
    strrpc(result,"\r","");
    strrpc(result,"\n","");
    //result[strlen(result)-1] = '\0';
	if(fclose(fp)) printf("file close error!\n");
    
    for(i = 0;i<t;i++){
        printf("neighbor:%s ,%d ,%s\n ,%f ,%d ,%s ,%f \n",\
        radio.neighbors[i].mac_addr,radio.neighbors[i].noise,radio.neighbors[i].rx_qam, \
        radio.neighbors[i].rx_rate,radio.neighbors[i].signal,radio.neighbors[i].tx_qam, \
        radio.neighbors[i].tx_rate);
        printf("************************\n");
    }
    //循环打印！！！
    printf("result %s\n",result);
    
    return result;
}

char * get_nei_infor_wlanconfig(char * result, struct radio_type radio){
    return NULL;
}




