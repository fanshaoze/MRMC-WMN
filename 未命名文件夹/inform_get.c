#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "meshclient.h"

int get_nodeid(){
    return 0;
}
int radios_inform_init(){
    //enable_all_radios()
    char * words;
    char wl[5];
    char word_temp[10];
    //本机命令行ifconfig，获取radio，判断该radio为2.4还是5，并且确定该radio的ID是wlan*
    
    char filename[] = "/home/fan/codelite/mesh-client/ifconfig"; //文件名
    //char filename[] = "/home/fan/wmn-mesh/client/ifconfig"; //文件名
    
    
    //changed
    FILE *fp;
    char StrLine[1024];             //每行最大读取的字符数
    int freq;
    int g_radio_num = 0;//表示2.4G射频的数量
    int ac_radio_num = 0;//表示5G射频的数量
    //int i = 0;
    int j = 0;
    //struct node_neighbor node;
    //以下数据用于的radios每个射频信息的赋值
    char radios_mac_addr[radio_max][50];//存储获取的所有射频的mac地址
    char radios_id[radio_max][20];//存储获取的射频的id  wlan0,wlan1,wlan2……
    //int radios_freq[radio_max];//存储获取的所有射频的频段 2.4/5
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return 0;
    }
    while (!feof(fp))
    {
        fgets(StrLine,1024,fp);  //读取一行
        words = strtok(StrLine," ");//按照空格划分一行的内容
        //获取标识的前四位，如果是wlan，证明这一行往下是一个无线网卡的信息
        //printf("init StrLine %s\n",StrLine);
        //printf("init words %s\n",words);
        char word[20];
        strcpy(word,words);
        strncpy(wl,word,3);
        wl[3] = '\0';
        if (strcmp(wl, "ath") == 0){
            strcpy(radios_id[j],words);//存储该radio的wlan标示到数组中
            words = tok_forward(words,4," ");
            printf("init0 words %s\n",words);
            strcpy(radios_mac_addr[j],words);//存储该radio的mac地址到mac数组中

            freq = get_freq(radios_id[j]);//根据该行是wlan*来获取可用的信道数量，进而判断是2.4还是5
            //printf("init j,freq,radios_mac_addr[j],%d,%d,%s\n",j,freq,radios_mac_addr[j]);
            radios_mac_addr[j][strlen(radios_mac_addr[j])] = '\0';
            printf("init j,freq,radios_mac_addr[j],%d,%d,%s\n",j,freq,radios_mac_addr[j]);
            //计算5G和2.4G的数量，这里以后可能会用于区分5G和2.4G，以后只用5G组网时使用
            if(freq == 5){
                if (strcmp(radios_id[j], "ath0") == 0){
                    strcpy(node_id,radios_mac_addr[j]);//默认使用wlan0的地址作为node_id
                    //node_id[strlen(node_id)-1] = '\0';
                    printf("init node_id,%s\n",node_id);
                }
                ac_radio_num += 1;
            }
            else if(freq == 2){//如果是2.4G
                if (strcmp(radios_id[j], "ath0") == 0){
                    strcpy(node_id,radios_mac_addr[j]);
                    //node_id[strlen(node_id)-1] = '\0';
                    printf("init node_id,%s\n",node_id);
                }
                g_radio_num += 1;
            }

            //radios_freq[j] = freq;//存储频段信息到数据中，
            //printf("^^^^^^^^^^^^^^^^node_id,%s\n",node_id);
           // printf("^^^^^^^^^^^^^^^^radios_freq[j],%d\n",radios_freq[j]);
            j++;
        }

        else continue;
    }
    fclose(fp);
    radio_no = ac_radio_num;
    snprintf(word_temp,3,"%s",node_id+15);
     snprintf(init_ssid , 5 , "%s","Link");
     strcat(init_ssid , word_temp);
    printf("init_ssid %s \n",init_ssid);
    //radio_no = ac_radio_num+g_radio_num;
    printf("init radio_no,%d\n",radio_no);
    //为radios分配空间
    
	return 0;
}

void radios_inform_init2(struct radio_type * radios){
    //enable_all_radios()
    char * words;
    char wl[5];
    //printf("^^^^^^^^^^^^^^^^^^^^\n");
    //本机命令行ifconfig，获取radio，判断该radio为2.4还是5，并且确定该radio的ID是wlan*

    //system("sh /home/fan/getradio.sh");
    //system("ifconfig > /root/ifconfig");
    //system("sh /root/getradios.sh");
    //char filename[] = "radios"; //文件名
    char filename[] = "/home/fan/codelite/mesh-client/ifconfig"; //文件名
   // char filename[] = "/root/ifconfig"; //文件名
    printf("init2 %s\n",filename);
    //char filename[] = "/home/fan/wmn-mesh/client/ifconfig"; //文件名


//changed
    FILE *fp;
    char StrLine[1024];             //每行最大读取的字符数
    int freq;
    int j = 0;
    int i = 0;
    int radio_no_all = 0;
    //struct node_neighbor node;
    //以下数据用于的radios每个射频信息的赋值
    char radios_mac_addr[radio_max][50];//存储获取的所有射频的mac地址
    char radios_id[radio_max][20];//存储获取的射频的id  wlan0,wlan1,wlan2……
    int radios_freq[radio_max];//存储获取的所有射频的频段 2.4/5
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return;
    }
    while (!feof(fp))
    {
        fgets(StrLine,1024,fp);  //读取一行
        //printf("%s\n", StrLine); //输出
        words = strtok(StrLine," ");//按照空格划分一行的内容
        //if(words)
            //printf("%s\n",words);
        //获取标识的前四位，如果是wlan，证明这一行往下是一个无线网卡的信息
        char word[20];
        strcpy(word,words);
        strncpy(wl,word,3);
        wl[3] = '\0';
        if (strcmp(wl, "ath") == 0){
            strcpy(radios_id[j],words);//存储该radio的wlan标示到数组中
            words = tok_forward(words,4," ");
            strcpy(radios_mac_addr[j],words);//存储该radio的mac地址到mac数组中
            freq = get_freq(radios_id[j]);//根据该行是wlan*来获取可用的信道数量，进而判断是2.4还是5
            radios_mac_addr[j][strlen(radios_mac_addr[j])] = '\0';
            radios_freq[j] = freq;//存储频段信息到数据中，
            j++;
        }
        else continue;
    }
    fclose(fp);
    radio_no_all = j;
    j = 0;
    //按顺序为每个radio赋值
    for(j = 0;j<radio_no_all;j++){
        if (radios_freq[j] == 2){
            continue;
        }
        strcpy(radios[i].radio_id,radios_id[j]);
        strcpy(radios[i].mac_addr,radios_mac_addr[j]);
        radios[i].freq = radios_freq[j];
		radios[i].mac_addr[strlen(radios[j].mac_addr)] = '\0';
        i++;

    }
	//为每个radio设置初始化的射频和信道，这里init没有让ssid和channel生效，只是赋值
    radio_init(radios);
    for(i = 0;i<radio_no;i++){
        printf("radios:\n,%s\n,%s\n,%s\n,%s\n",radios[i].channel,radios[i].mac_addr,radios[i].ssid,radios[i].radio_id);
        printf("init2 ************************\n");
    }
    //printf("%d\n",radio_no);
    //循环打印！！！！
    //printf("radios:0\n,%s\n,%s\n,%s\n,%s\n",radios[0].channel,radios[0].mac_addr,radios[0].ssid,radios[0].radio_id);
   // printf("fenge************************\n");
    //printf("radios:1\n,%s\n,%s\n,%s\n,%s\n",radios[1].channel,radios[1].mac_addr,radios[1].ssid,radios[1].radio_id);
    //return radios;
}

int get_freq(char * ath){

    char filename[100];
    if(strcmp(ath,"ath0" )== 0){
        strcpy(filename, "/home/fan/codelite/mesh-client/iwlistath0"); //文件名
    }
    if(strcmp(ath,"ath1") == 0){
        strcpy(filename, "/home/fan/codelite/mesh-client/iwlistath1"); //文件名
    }
    FILE *fp;
    char * words;
    char channel_number[2];
    char StrLine[1024];

//changed change the command wlan -- ath
              //每行最大读取的字符数
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return -1;
    }
    if (!feof(fp)) {
        fgets(StrLine,1024,fp);  //读取一行
        words = strtok(StrLine," ");
        words = strtok(NULL," ");
        strcpy(channel_number,words);
        int ch_number=atoi(channel_number);
        printf("ch_number is %d\n",ch_number);
        if (ch_number<20){
            //printf("the freq of this is 2.4\n");
            return 2;
        }
        else if (ch_number>=20){
            //printf("the freq of this is 5\n");
            return 5;
        }
        else{
            printf("error freq\n");
            return 0;
        }
    }
	if(fclose(fp)) printf("file close error!\n");
    fp = NULL;
    return 0;
}



int get_neighbor(int clientSocket,struct radio_type * radios){
    //使能所有的射频
    FILE* fp = NULL; // 文件指针
    
	//char* szAppendStr = "Text";
	//errno_t eResult;
    if((fp = fopen("temp.txt","w+")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return 0;
    }
    //recover  enable_all_radios(radios);
    //wait!
    char neigh_send_inform[1000];
    char radio_result[1000];
    int i = 0;
    for(i = 0;i<radio_no;i++){
        neigh_send_inform[0] = '\0';
        radio_result[0] = '\0';
        strcpy(radio_result, get_nei_infor(radio_result,radios[i]));

		strcat(neigh_send_inform,"NEIGHBOR ");

        strcat(neigh_send_inform,node_id);
       

        strcat(neigh_send_inform," ");
        strcat(neigh_send_inform,radios[i].mac_addr);
        
        
        strcat(neigh_send_inform," ");
        strcat(neigh_send_inform,radios[i].channel);
        
        
        strcat(neigh_send_inform," ");
        strcat(neigh_send_inform, radio_result);
        
        strcat(neigh_send_inform, "\r\n");
      
        
        //fputs("~~~~",fp);
        printf("neigh_send_inform：\n%s\n",neigh_send_inform);
        fputs(neigh_send_inform, fp);
        send(clientSocket, neigh_send_inform, strlen(neigh_send_inform), 0);
        
    }
	fclose(fp);
    return 0;
}


char * get_nei_infor(char * result, struct radio_type radio){
    printf("radio.radio_id %s\n",radio.radio_id);
    //struct node_neighbor nei_node[];
    char filename[50];
        
    /***********************
    char filename[] = "neighbors"; //文件名
    char * orig_com = "sh /home/fan/shelltest.sh";
    ***************/
    int z = 0;
    int i = 0;
    int recordflag = 0;//用于标识“以下信噪比等信息是否需要记录”
    if(strcmp(radio.radio_id,"ath0") == 0)
        snprintf(filename,50,"%s","/home/fan/codelite/mesh-client/ath0scanning"); //文件名
    if(strcmp(radio.radio_id,"ath1") == 0)
        snprintf(filename,50,"%s","/home/fan/codelite/mesh-client/ath1scanning"); //文件名
    snprintf(filename,50,"%s","/home/fan/codelite/mesh-client/ath0scanning"); //temp delete
    FILE *fp;
    char *words;
    char * words_next;
    char words_temp[200];
    char mac_addr[1050];
    //char rx_rate_str[20];
    //char tx_rate_str[20];
    char signal[20];
    char StrLine[1024];
    char flag[20];
    int station_number = 0;

    //************system(orig_com);
    //wait
	sleep(5);
    fp = fopen(filename,"r");
	if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return "error";
    }
    //changed
    while (!feof(fp)) {
        fgets(StrLine,1024,fp);  //读取一行
        char * words_next;
        words_next = strstr(StrLine,"ESSID");
        snprintf(words_temp , 20 , "%s","ESSID:\"");
        strcat(words_temp , radio.ssid);
        strcat(words_temp , "\"");
        printf("words_temp %s \n",words_temp);
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
       

    //station_number-=1;
    printf("station_number %d\n",station_number);
    
    radio.neighbors = (struct node_neighbor*) malloc(sizeof(struct node_neighbor)*station_number);
    //rewind(fp);//回到文件头部，重新扫描，并保存邻居信息
    
    int t =0;
    strcpy(words_temp,"");
    
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
    ////需要修改，所有邻居信息的初始化
    
    
//    rewind(fp);//回到文件头部，重新扫描，并保存邻居信息
    if(fclose(fp)) printf("file close error!\n");
    fp = NULL;
   // fp = fopen(filename,"r");
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return "";
    }
    //fgets(StrLine,1024,fp);  //读取一行,第一行是ath
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
            printf("words,%s\n",words);
            snprintf(mac_addr,50,"%s",words);
            printf("get current mac_addr %s\n",mac_addr);
           
        }
        else if(strcmp(flag, "ESSI") == 0){
            printf("ESSID  words,%s\n",words);
            snprintf(flag,5,"%s",words+7);
            printf("link %s \n",flag);
            if (strcmp(flag, "Link") == 0){
                snprintf(words_temp , 20 , "%s","ESSID:\"");
                strcat(words_temp , radio.ssid);
                strcat(words_temp , "\"");
                printf("words_temp %s \n",words_temp);
                char temp2[20];
                
                snprintf(temp2 , 20 , "%s",words);
                strrpc(temp2,"\n","");
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

    
    /*

    while (!feof(fp)) {
        fgets(StrLine,1024,fp);  //读取一行
		char str_tmp[1024];
		strcpy(str_tmp,StrLine);
        words = strtok(StrLine," ");
        strcpy(flag,words);
        strrpc(flag,"\t","");
        strrpc(flag,"\r","");
        strrpc(flag,"\n","");
        printf("getsg  %s \n",flag);
        if (strcmp(flag, "Station") == 0){
           // station_number+=1;
			words = tok_forward(words,1," ");
            strcpy(mac_addr,words);
            //words = tok_forward(words,1," ");
            strcpy(radio.neighbors[t].mac_addr,mac_addr);//获取mac地址
           
        }
        else if(strcmp(flag, "signal:") == 0){
            words = tok_forward(words,1," ");
            strcpy(signal,words);
            
            //char * signal_splite = strtok(,"M");
            int num=atoi(signal);
            radio.neighbors[t].signal = num;//获取信号强度
			radio.neighbors[t].noise = -95;
        }
        else if(strcmp(flag, "tx") == 0){
             words = tok_forward(words,1," ");
             printf("tx %s\n",words);
             strcpy(words_temp,words);
             
             if(strlen(words)>=8)
                strncpy(word,words,8);
            else strcpy(word," ");
            if(strcmp(word,"bitrate:") == 0){
                word[0] = '\0';
                strcpy(word,words_temp);
                 printf("gets %s\n",word);
                //word = tok_forward(word,1," ");
                 strncpy(tx_rate_str,word+9,strlen(word)-9);
                //strcpy(rx_rate_str,words);
                int num=atoi(tx_rate_str);
                radio.neighbors[t].tx_rate = num;
                char * words_next;
				printf("num %f\n",radio.neighbors[t].tx_rate);
                char tx_qam_str[100] = "\0";
                words_next = strstr(str_tmp,"MBit/s ");
                strncpy(tx_qam_str,words_next+7,strlen(words_next)-8);
                words_next = strtok(str_tmp,"s");
                strrpc(tx_qam_str," ","$");
				//rx_qam_str[strlen(words_next)-2] = '\0';
                strcpy(radio.neighbors[t].tx_qam,tx_qam_str);//获取rx速率
                printf("qam %s\n", radio.neighbors[t].tx_qam);
                word[0] = '\0';
            }
        }
        else if(strcmp(flag, "rx") == 0){
             words = tok_forward(words,1," ");
             printf("%s\n",words);
             strcpy(words_temp,words);
             //char word[200];
             if(strlen(words)>=8){
                strncpy(word,words,8);
                word[8]='\0';
             }
            else strcpy(word," ");
            if(strcmp(word,"bitrate:") == 0){
                word[0] = '\0';
                strcpy(word,words_temp);
                 printf("gets %s\n",word);
                //word = tok_forward(word,1," ");
                 strncpy(rx_rate_str,word+9,strlen(word)-9);
                //strcpy(rx_rate_str,words);
                int num=atoi(rx_rate_str);
                radio.neighbors[t].rx_rate = num;
                char * words_next;
				printf("num %f\n",radio.neighbors[t].rx_rate);
                char rx_qam_str[100] = "\0";
                words_next = strstr(str_tmp,"MBit/s ");
                strncpy(rx_qam_str,words_next+7,strlen(words_next)-8);
                words_next = strtok(str_tmp,"s");
                strrpc(rx_qam_str," ","$");
				//rx_qam_str[strlen(words_next)-2] = '\0';
                strcpy(radio.neighbors[t].rx_qam,rx_qam_str);//获取rx速率
                printf("qam %s\n", radio.neighbors[t].rx_qam);
                word[0] = '\0';
            }
        }
        else if(strcmp(flag, "authorized:yes") == 0){
            //radio.neighbors[t];
            
            strcat(result,compose_neighbor(radio.neighbors[t]));
            strcat(result," ");
			t++;
        }
    }
    result[strlen(result)-1] = '\0';
	if(fclose(fp)) printf("file close error!\n");
    int i = 0;
    for(i = 0;i<t;i++){
        printf("neighbor:%s ,%d ,%s\n ,%f ,%d ,%s ,%f \n",\
        radio.neighbors[i].mac_addr,radio.neighbors[i].noise,radio.neighbors[i].rx_qam, \
        radio.neighbors[i].rx_rate,radio.neighbors[i].signal,radio.neighbors[i].tx_qam, \
        radio.neighbors[i].tx_rate);
        printf("************************\n");
    }
    //循环打印！！！
    //printf("fenge************************\n");
    
    return result;
}
 * */
