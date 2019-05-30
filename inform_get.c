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

int get_freq(char * wlan){


 /***********************
    char * orig_com = "sh /home/fan/gechannels.sh";
    char *sh_com = (char *) malloc(strlen(orig_com) + strlen(wlan));

    strcpy(sh_com, orig_com);
    strcat(sh_com, wlan);
    system(sh_com);
   */
    char command[100];
    sprintf(command,"%s%s%s%s%s","iwlist ",wlan," channel > /tmp/",wlan,"freq");
    char filename[100];
    
    system(command);
    sprintf(filename,"%s%s%s","/tmp/",wlan,"freq");
    printf("freq %s\n",filename);
    /*
    if(strcmp(wlan,"wlan0" )== 0){
        strcpy(filename, "/home/fan/codelite/mesh-client/iwlistwlan0"); //文件名
    }
    if(strcmp(wlan,"wlan1") == 0){
        strcpy(filename, "/home/fan/codelite/mesh-client/iwlistwlan1"); //文件名
    }
    */
    FILE *fp;
    char * words;
    char channel_number[3];
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
        snprintf(channel_number,4,"%s",words);
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


int get_neighbor( float* loads,int clientSocket,struct radio_type * radios){
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
        if(strcmp(neicommand,command0) == 0) {
            snprintf(radio_result,strlen(radio_result)+1,"%s",get_nei_infor_iw(radio_result,radios[i]));
        }
        if(strcmp(neicommand,command1) == 0) {
            snprintf(radio_result,strlen(radio_result)+1,"%s",get_nei_infor_iwlist(radio_result,radios[i]));
        }
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


char * get_nei_infor_iw(char * result, struct radio_type radio){
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

char * get_nei_infor_iwlist(char * result, struct radio_type radio){
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
                char tx_qam_str[100] = "\0";
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
                char rx_qam_str[100] = "\0";
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



float* getload(struct radio_type * radios){
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
