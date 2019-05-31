#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "meshclient.h"

int get_nodeid_864(){
    return 0;
}
int radios_inform_init_864(){
    //enable_all_radios()
    char * words;
    char wl[5];
    char word_temp[10];
    //本机命令行ifconfig，获取radio，判断该radio为2.4还是5，并且确定该radio的ID是wlan*

    //system("sh /home/fan/getradio.sh");
    system("ifconfig > /tmp/ifconfig");
    //system("sh /root/getradios.sh");
    //char filename[] = "radios"; //文件名
    
    char filename[] = "/tmp/ifconfig"; //文件名
    printf("init %s\n",filename);
    
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
        snprintf(word,20,"%s",words);
        snprintf(wl,4,"%s",word);
        wl[3] = '\0';
        if (strcmp(wl, "ath") == 0){
            snprintf(radios_id[j],strlen(words)+1,"%s",words);
            words = tok_forward(words,4," ");
            printf("init0 words %s\n",words);
            snprintf(radios_mac_addr[j],strlen(words)+1,"%s",words);//存储该radio的mac地址到mac数组中
            freq = get_freq(radios_id[j]);//根据该行是wlan*来获取可用的信道数量，进而判断是2.4还是5
            //printf("init j,freq,radios_mac_addr[j],%d,%d,%s\n",j,freq,radios_mac_addr[j]);
            radios_mac_addr[j][strlen(radios_mac_addr[j])] = '\0';
            printf("init j,freq,radios_mac_addr[j],%d,%d,%s\n",j,freq,radios_mac_addr[j]);
            //计算5G和2.4G的数量，这里以后可能会用于区分5G和2.4G，以后只用5G组网时使用
            if(freq == 5){
                if (strcmp(radios_id[j], "ath0") == 0){
                    snprintf(node_id,strlen(radios_mac_addr[j])+1,"%s",radios_mac_addr[j]);//默认使用wlan0的地址作为node_id
                    //node_id[strlen(node_id)-1] = '\0';
                    printf("init node_id,%s\n",node_id);
                }
                ac_radio_num += 1;
            }
            else if(freq == 2){//如果是2.4G
                if (strcmp(radios_id[j], "ath0") == 0){
                     snprintf(node_id,strlen(radios_mac_addr[j])+1,"%s",radios_mac_addr[j]);//默认使用wlan0的地址作为node_id
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
    
    printf("init radio_no,%d\n",radio_no);
    //为radios分配空间
    
	return 0;
}

void radios_inform_init2_864(struct radio_type * radios){
    //enable_all_radios()
    char * words;
    char wl[5];
    //printf("^^^^^^^^^^^^^^^^^^^^\n");
    //本机命令行ifconfig，获取radio，判断该radio为2.4还是5，并且确定该radio的ID是wlan*

    //system("sh /home/fan/getradio.sh");
    system("ifconfig > /tmp/ifconfig");
    //system("sh /root/getradios.sh");
    //char filename[] = "radios"; //文件名
    ///home/fan/codelite/mesh-client
    char filename[] = "/tmp/ifconfig"; //文件名
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
        snprintf(word,20,"%s",words);
        snprintf(wl,4,"%s",word);
        wl[3] = '\0';
        if (strcmp(wl, "ath") == 0){
            snprintf(radios_id[j],strlen(words)+1,"%s",words);
            words = tok_forward(words,4," ");
            snprintf(radios_mac_addr[j],strlen(words)+1,"%s",words);//存储该radio的mac地址到mac数组中
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
        snprintf(radios[i].radio_id,strlen(radios_id[j])+1,"%s",radios_id[j]);
        snprintf(radios[i].mac_addr,strlen(radios_mac_addr[j])+1,"%s",radios_mac_addr[j]);
        radios[i].freq = radios_freq[j];
        radios[i].mac_addr[strlen(radios[j].mac_addr)] = '\0';
        i++;

    }
    //为每个radio设置初始化的射频和信道，这里init没有让ssid和channel生效，只是赋值
    radio_init_864(radios);
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


int get_neighbor_864( float* loads,int clientSocket,struct radio_type * radios){
    //使能所有的射频
    FILE* fp = NULL; // 文件指针
	//char* szAppendStr = "Text";
	//errno_t eResult;
    if((fp = fopen("/tmp/temp","w+")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return 0;
    }
//    enable_all_radios(radios);
    char neigh_send_inform[1000];
    char radio_result[1000];
    char load_tmp[10];
    int i = 0;
    for(i = 0;i<radio_no;i++){
        neigh_send_inform[0] = '\0';
        radio_result[0] = '\0';
        radios[i].load = loads[i];
        if(strcmp(neicommand,command0)==0){
            snprintf(radio_result,strlen(radio_result)+1,"%s",get_nei_infor_iw(radio_result,radios[i]));
        }
        else if(strcmp(neicommand,command1)==0){
            snprintf(radio_result,strlen(radio_result)+1,"%s",get_nei_infor_iwlist(radio_result,radios[i]));
        }
        else return 0;


		strcat(neigh_send_inform,"NEIGHBOR ");

        strcat(neigh_send_inform,node_id);
       

        strcat(neigh_send_inform," ");
        strcat(neigh_send_inform,radios[i].mac_addr);
        
        
        strcat(neigh_send_inform," ");
        snprintf(load_tmp,10,"%f",radios[i].load);
        strcat(neigh_send_inform,load_tmp);
        
        
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




float* getload_864(struct radio_type * radios){
    float * loads = (float*)malloc(sizeof(float)*radio_no);    
    int * disables = (int*)malloc(sizeof(int)*radio_no);
    int i = 0;
    for(i = 0;i<radio_no;i++){
        disables[i] = radios[i].disabled;
    }
    char * words;
    char wl[5];
    char trans[5];
    char word_temp[20];
    int flag = 0;
    char * words_next;
    double RX_bytes;
    double TX_bytes;
    /*
    int packets_r = 0;
    int errors_r = 0;
    int dropped_r= 0;
    int packets_t = 0;
    int errors_t = 0;
    int dropped_t= 0;
     * */
    int index = 0;

    //本机命令行ifconfig，获取radio，判断该radio为2.4还是5，并且确定该radio的ID是wlan*

    //system("sh /home/fan/getradio.sh");
    system("ifconfig > /tmp/ifconfig");
    //system("sh /root/getradios.sh");
    //char filename[] = "radios"; //文件名
    
    char filename[] = "/tmp/ifconfig"; //文件名
    printf("init %s\n",filename);
    
    FILE *fp;
    char StrLine[1024];             //每行最大读取的字符数
    char StrLine_tmp[1024];
    //int i = 0;
    //struct node_neighbor node;
    //以下数据用于的radios每个射频信息的赋值
    //int radios_freq[radio_max];//存储获取的所有射频的频段 2.4/5
    if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("error!");
        return 0;
    }
    while (!feof(fp))
    {
        fgets(StrLine,1024,fp);  //读取一行
        snprintf(StrLine_tmp,1024,"%s",StrLine);
        words = strtok(StrLine," ");//按照空格划分一行的内容
        //获取标识的前四位，如果是wlan，证明这一行往下是一个无线网卡的信息
        //printf("init StrLine %s\n",StrLine);
        //printf("init words %s\n",words);
        char word[20];
        snprintf(word,20,"%s",words);
        snprintf(wl,4,"%s",word);
        snprintf(trans,3,"%s",word);
        
        wl[3] = '\0';
        if (strcmp(wl, "ath") == 0){
            for(i = 0;i<radio_no;i++){
                if(strcmp(words, radios[i].radio_id) == 0){
                    words_next = strstr(words,"h");
                    snprintf(word_temp,strlen(words_next),"%s",words_next+1);
                    index = atoi(word_temp);
                }
            }
            flag = 1;
        }
        if(flag == 1 && strcmp(trans, "RX") == 0){
                words = tok_forward(words,1," ");
                words_next = strstr(words,":");
                snprintf(word_temp,strlen(words)-strlen(words_next)+1,"%s",words);
            if(strcmp(word_temp, "bytes") == 0){
                snprintf(word_temp,strlen(words_next)+1,"%s",words_next);
                RX_bytes = atof(word_temp);
                
                words = tok_forward(words,1," ");
                words_next = strstr(words,":");
                snprintf(word_temp,strlen(words_next)+1,"%s",words_next);
                TX_bytes = atof(word_temp);
                
                if(RX_bytes>TX_bytes) loads[index] = RX_bytes;
                else loads[index] = TX_bytes;
                /*
                    words = tok_forward(words,1," ");
                    words_next = strstr(words,":");
                    snprintf(word_temp,strlen(words_next),"%s",words_next+1);
                    packets_r = atoi(word_temp);
                    
                    words = tok_forward(words,1," ");
                    words_next = strstr(words,":");
                    snprintf(word_temp,strlen(words_next),"%s",words_next+1);
                    errors_r = atoi(word_temp);
                    
                    words = tok_forward(words,1," ");
                    words_next = strstr(words,":");
                    snprintf(word_temp,strlen(words_next),"%s",words_next+1);
                    dropped_r = atoi(word_temp);
                     * */
            }
            flag = 0;
        }
        else continue;
    }
    fclose(fp); 
    for(i = 0;i<radio_no;i++){
        if(radios[i].disabled == 1){
            loads[i] = 0;
        }
    }
    //先判断射频disable，再获取 负载信息
    return loads;
}
