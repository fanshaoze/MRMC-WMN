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
    //本机命令行ifconfig，获取radio，判断该radio为2.4还是5，并且确定该radio的ID是wlan*

    //system("sh /home/fan/getradio.sh");
    //system("sh /root/getradios.sh");
    //char filename[] = "radios"; //文件名
    
    char filename[] = "/home/fan/codelite/mesh-client/ifconfig"; //文件名
    //char filename[] = "/home/fan/wmn-mesh/client/ifconfig"; //文件名
    FILE *fp;
    char StrLine[1024];             //每行最大读取的字符数
    int freq;
    int g_radio_num = 0;//表示2.4G射频的数量
    int ac_radio_num = 0;//表示5G射频的数量
    int i = 0;
    int j = 0;
    //struct node_neighbor node;
    //以下数据用于的radios每个射频信息的赋值
    char radios_mac_addr[radio_max][50];//存储获取的所有射频的mac地址
    char radios_id[radio_max][20];//存储获取的射频的id  wlan0,wlan1,wlan2……
    int radios_freq[radio_max];//存储获取的所有射频的频段 2.4/5
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return NULL;
    }
    while (!feof(fp))
    {
        fgets(StrLine,1024,fp);  //读取一行
        words = strtok(StrLine," ");//按照空格划分一行的内容
        //获取标识的前四位，如果是wlan，证明这一行往下是一个无线网卡的信息
        char word[10];
        strcpy(word,words);
        strncpy(wl,word,4);
        wl[4] = '\0';
        if (strcmp(wl, "wlan") == 0){
            strcpy(radios_id[j],words);//存储该radio的wlan标示到数组中
            words = tok_forward(words,4," ");
            strcpy(radios_mac_addr[j],words);//存储该radio的mac地址到mac数组中

            freq = get_freq(radios_id[j]);//根据该行是wlan*来获取可用的信道数量，进而判断是2.4还是5
            //printf("radios_mac_addr[j],%d,%s\n",j,radios_mac_addr[j]);
            radios_mac_addr[j][strlen(radios_mac_addr[j])-2] = '\0';
            //计算5G和2.4G的数量，这里以后可能会用于区分5G和2.4G，以后只用5G组网时使用
            if(freq == 5){
                if (strcmp(radios_id[j], "wlan0") == 0){
                    strcpy(node_id,radios_mac_addr[j]);//默认使用wlan0的地址作为node_id
                    //node_id[strlen(node_id)-1] = '\0';
                }
                ac_radio_num += 1;
            }
            else if(freq == 2){//如果是2.4G
                if (strcmp(radios_id[j], "wlan0") == 0){
                    strcpy(node_id,radios_mac_addr[j]);
                    //node_id[strlen(node_id)-1] = '\0';
                }
                g_radio_num += 1;
            }
            radios_freq[j] = freq;//存储频段信息到数据中，
            //printf("^^^^^^^^^^^^^^^^node_id,%s\n",node_id);
           // printf("^^^^^^^^^^^^^^^^radios_freq[j],%d\n",radios_freq[j]);
            j++;
        }

        else continue;
    }
    fclose(fp);
    //radio_no = ac_radio_num;
    radio_no = ac_radio_num+g_radio_num;
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
    //system("sh /root/getradios.sh");
    //char filename[] = "radios"; //文件名
    ///home/fan/codelite/mesh-client
    char filename[] = "/home/fan/codelite/mesh-client/ifconfig"; //文件名
    //char filename[] = "/home/fan/wmn-mesh/client/ifconfig"; //文件名

    FILE *fp;
    char StrLine[1024];             //每行最大读取的字符数
    int freq;
    int g_radio_num = 0;//表示2.4G射频的数量
    int ac_radio_num = 0;//表示5G射频的数量
    int i = 0;
    int j = 0;
    //struct node_neighbor node;
    //以下数据用于的radios每个射频信息的赋值
    char radios_mac_addr[radio_max][50];//存储获取的所有射频的mac地址
    char radios_id[radio_max][20];//存储获取的射频的id  wlan0,wlan1,wlan2……
    int radios_freq[radio_max];//存储获取的所有射频的频段 2.4/5
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return NULL;
    }
    while (!feof(fp))
    {
        fgets(StrLine,1024,fp);  //读取一行
        //printf("%s\n", StrLine); //输出
        words = strtok(StrLine," ");//按照空格划分一行的内容
        //if(words)
            //printf("%s\n",words);
        //获取标识的前四位，如果是wlan，证明这一行往下是一个无线网卡的信息
        char word[10];
        strcpy(word,words);
        strncpy(wl,word,4);
        wl[4] = '\0';
        if (strcmp(wl, "wlan") == 0){
            strcpy(radios_id[j],words);//存储该radio的wlan标示到数组中
            words = tok_forward(words,4," ");
            strcpy(radios_mac_addr[j],words);//存储该radio的mac地址到mac数组中
            freq = get_freq(radios_id[j]);//根据该行是wlan*来获取可用的信道数量，进而判断是2.4还是5
            radios_mac_addr[j][strlen(radios_mac_addr[j])-1] = '\0';
            radios_freq[j] = freq;//存储频段信息到数据中，
            j++;
        }

        else continue;
    }
    fclose(fp);
    //按顺序为每个radio赋值
    for(j = 0;j<radio_no;j++){
        strcpy(radios[j].radio_id,radios_id[j]);
        strcpy(radios[j].mac_addr,radios_mac_addr[j]);
        radios[j].freq = radios_freq[j];
		radios[j].mac_addr[strlen(radios[j].mac_addr)-1] = '\0';

    }
	//为每个radio设置初始化的射频和信道，这里init没有让ssid和channel生效，只是赋值
    radio_init(radios);
    //printf("%d\n",radio_no);
    //循环打印！！！！
    //printf("radios:0\n,%s\n,%s\n,%s\n,%s\n",radios[0].channel,radios[0].mac_addr,radios[0].ssid,radios[0].radio_id);
   // printf("fenge************************\n");
    //printf("radios:1\n,%s\n,%s\n,%s\n,%s\n",radios[1].channel,radios[1].mac_addr,radios[1].ssid,radios[1].radio_id);
    //return radios;
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
    enable_all_radios(radios);
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





int get_freq(char * wlan){


 /***********************
    char * orig_com = "sh /home/fan/gechannels.sh";
    char *sh_com = (char *) malloc(strlen(orig_com) + strlen(wlan));

    strcpy(sh_com, orig_com);
    strcat(sh_com, wlan);
    system(sh_com);
   */

    char filename[100];
    if(strcmp(wlan,"wlan0" )== 0){
        strcpy(filename, "/home/fan/codelite/mesh-client/iwlistwlan0"); //文件名
    }
    if(strcmp(wlan,"wlan1") == 0){
        strcpy(filename, "/home/fan/codelite/mesh-client/iwlistwlan1"); //文件名
    }
    FILE *fp;
    char * words;
    char channel_number[2];
    char StrLine[1024];

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
        if (strcmp(channel_number, "11") == 0){
            //printf("the freq of this is 2.4\n");
            return 2;
        }
        else if (strcmp(channel_number, "25") == 0){
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

char * get_nei_infor(char * result, struct radio_type radio){
    printf("radio.radio_id %s\n",radio.radio_id);
    //struct node_neighbor nei_node[];
    char filename[100];
    /***********************
    char filename[] = "neighbors"; //文件名
    char * orig_com = "sh /home/fan/shelltest.sh";
    ***************/
    if(strcmp(radio.radio_id,"wlan0") == 0)
        strcpy(filename,"/home/fan/codelite/mesh-client/dump"); //文件名
    if(strcmp(radio.radio_id,"wlan1") == 0)
        strcpy(filename,"/home/fan/codelite/mesh-client/dump"); //文件名
    filename[strlen("/home/fan/codelite/mesh-client/dump")] = '\0';
    FILE *fp;
    char * words;
    char mac_addr[1050];
    char rx_rate_str[20];
    char tx_rate_str[20];
    char signal[20];
    char StrLine[1024];
    char flag[20];
    int station_number = 0;

    //************system(orig_com);
	
    fp = fopen(filename,"r");
	if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return -1;
    }
    while (!feof(fp)) {
        fgets(StrLine,1024,fp);  //读取一行
        words = strtok(StrLine," ");
        strcpy(flag,words);
        if (strcmp(flag, "Station") == 0){
            words = tok_forward(words,1," ");
            station_number+=1;//累积，用于计算共有多少个邻居节点，并分配空间
            strcpy(mac_addr,words);
            words = NULL;
        }
    }
    radio.neighbors = (struct node_neighbor*) malloc(sizeof(struct node_neighbor)*station_number);
    rewind(fp);//回到文件头部，重新扫描，并保存邻居信息
    int t =0;
    while (!feof(fp)) {
        fgets(StrLine,1024,fp);  //读取一行
		char str_tmp[1024];
		strcpy(str_tmp,StrLine);
        words = strtok(StrLine," ");
        strcpy(flag,words);
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
            if(strcmp(words,"bitrate:") == 0){
                words = tok_forward(words,1," ");
                strcpy(tx_rate_str,words);
                int num=atoi(tx_rate_str);
                radio.neighbors[t].tx_rate = num;
                char * words_next;
                char tx_qam_str[100] = "\0";
                words_next = strstr(str_tmp,"MBit/s ");
                strncpy(tx_qam_str,words_next+7,strlen(words_next)-9);
				strrpc(tx_qam_str," ","$");
                strcpy(radio.neighbors[t].tx_qam,tx_qam_str);//获取tx速率
            }
        }
        else if(strcmp(flag, "rx") == 0){
             words = tok_forward(words,1," ");
            if(strcmp(words,"bitrate:") == 0){
                words = tok_forward(words,1," ");
                strcpy(rx_rate_str,words);
                int num=atoi(rx_rate_str);
                radio.neighbors[t].rx_rate = num;
                char * words_next;
				//printf("strline %s\n",str_tmp);
                char rx_qam_str[100] = "\0";
                words_next = strstr(str_tmp,"MBit/s ");
                strncpy(rx_qam_str,words_next+7,strlen(words_next)-9);
                words_next = strtok(str_tmp,"s");
                strrpc(rx_qam_str," ","$");
				//rx_qam_str[strlen(words_next)-2] = '\0';
                strcpy(radio.neighbors[t].rx_qam,rx_qam_str);//获取rx速率
            }
        }
        else if(strcmp(flag, "authorized:") == 0){
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
