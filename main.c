/*socket tcp客户端*/
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "meshclient.h"
/*
连接到服务器后，会不停循环，等待输入，
输入quit后，断开与服务器的连接
*/
int main()
{
    
    printf("start\n");
    int com_no = 0;
    
    int i = 0;
    struct radio_type * radios = NULL;
    
    //通过扫描，获取所有radio的信息，存储在radios的数组中
    float* loads = NULL;
    //初始化全局变量
    radio_no = 2;
    enable_time=120;
    findnei_time = 20;
    snprintf(ipaddr,20,"%s","NULL");
    snprintf(hardware,20,"%s","WPJ428");
    snprintf(neicommand,20,"%s","iw");
    
    //snprintf(hardware,20,"%s","WPQ864");
    //snprintf(neicommand,20,"%s","iwlist");
    
    char filename[] = "/config"; 
             //每行最大读取的字符数
    char * words;
    char StrLine[1024];    
    FILE *fp;
     if((fp = fopen(filename,"r")) == NULL) //判断文件是否存在及可读
    {
        printf("no config!");
        return 0;
    }
    while (!feof(fp))
    {

    fgets(StrLine,1024,fp);  //读取一行
	char str_tmp[1024];
    snprintf(str_tmp,1024,"%s",StrLine);
	strrpc(str_tmp,"\n","\0");
    printf("Str %s %s\n",StrLine,str_tmp);
	words = strtok(str_tmp,":");//按照空格划分一行的内容
        printf("words %s\n",words);
	if(strncmp(words, "Enable",6) == 0){
        enable_time = atoi(tok_forward(words,1,":"));
    }
    else if(strncmp(words, "NeighborFind",12) == 0){
        findnei_time = atoi(tok_forward(words,1,":"));
    }
    else if(strncmp(words, "ServerIp",8) == 0){
        snprintf(ipaddr,20,"%s",tok_forward(words,1,":"));
    }
    else if(strncmp(words, "hardware",8) == 0){
        snprintf(hardware,20,"%s",tok_forward(words,1,":"));
    }
    else if(strncmp(words, "neicommand",10) == 0){
        snprintf(neicommand,20,"%s",tok_forward(words,1,":"));
    }
	else printf("wrong config");
    }
    printf("enble %d,findnei_time %d,ServerIp %s,hardware %s,neicommand %s\n,",enable_time,findnei_time,ipaddr,hardware,neicommand);

    snprintf(init_ssid,6,"%s","Link1");
    radios_inform_init();
	radios = (struct radio_type*) malloc(sizeof(struct radio_type)*radio_no);
    int *disables = (int *)malloc(sizeof(int) * radio_no);
    for (i = 0;i<radio_no;i++){
        disables[i] = 0;
    }
    for(i = 0;i<radio_no;i++){
        snprintf(radios[i].radio_id,10,"%s","NULL");
        snprintf(radios[i].mac_addr,50,"%s","NULL");
        snprintf(radios[i].channel,10,"%s","NULL");
        snprintf(radios[i].ssid,10,"%s","NULL");
        radios[i].neigh_count = 0;
        radios[i].freq = 0;
        radios[i].disabled = 0;
        radios[i].neighbors = NULL;
    }
	radios_inform_init2(radios);
    alloc_config_all(radios);
    printf("sleep begin");
    confirm_wireless();
    printf("sleep over");
    char sendbuf[200];
	//char recvbuf[200];

//客户端只需要一个套接字文件描述符，用于和服务器通信
	int clientSocket;
	//描述服务器的socket
	struct sockaddr_in serverAddr;
	//int iDataNum = 0;
    if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	perror("socket");
    	return 1;
	}
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(sendport);
    //指定服务器端的ip，本地测试：192.168.1.152
    //inet_addr()函数，将点分十进制IP转换成网络字节序IP
    serverAddr.sin_addr.s_addr = inet_addr(ipaddr);
    //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
    	perror("connect");
    	return 1;
    }
    printf("connect socket1...\n");
    printf("发送消息:");
    snprintf(sendbuf,strlen(sendbuf),"%s","send neighbor information\r\n");
    //scanf("%s", sendbuf);
    
    get_neighbor( loads,clientSocket,radios);
    
    
    int clientSocket1;
	//描述服务器的socket
	struct sockaddr_in serverAddr1;
	char sendbuf1[200];
	char recvbuf1[300];
    char recvbuf2[300];
	int iDataNum1;
	if((clientSocket1 = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
    	perror("socket");
    	return 1;
	}
    serverAddr1.sin_family = AF_INET;
    serverAddr1.sin_port = htons(receiveport);
    //指定服务器端的ip，本地测试：127.0.0.1
    //inet_addr()函数，将点分十进制IP转换成网络字节序IP
    serverAddr1.sin_addr.s_addr = inet_addr(ipaddr);
    if(connect(clientSocket1, (struct sockaddr *)&serverAddr1, sizeof(serverAddr1)) < 0)
    {
    	perror("connect");
    	return 1;
    }
    printf("connect socket1...\n");
  
    //strcpy(sendbuf1,node_id);
    sprintf(sendbuf1,"%s%s",node_id,"\r\n");
    printf("node_ID %s",sendbuf1);
    
    send(clientSocket1, sendbuf1, strlen(sendbuf1), 0);
    
    recvbuf1[0] = '\0';
    

    while(1)

    {


        //radios_inform_init(radios);
   		//if(strcmp(sendbuf1, "quit") == 0)
    		//break;
    	printf("get message:");
    	
    	iDataNum1 = recv(clientSocket1, recvbuf1, 300, 0);
        //从服务器端接收命令
    	recvbuf1[iDataNum1] = '\0';
        //begin
        //strcpy(recvbuf1,"DISCOVER");
        printf("%s\n", recvbuf1);
        snprintf(recvbuf2,strlen(recvbuf2),"%s",recvbuf1);
        //recvbuf1[strlen("DISCOVER")] = '\0';
        //printf("%s\n", recvbuf1);
        //end
        //解析命令，将命令和标识相对应，
        com_no = decode_command(recvbuf2);
        //char com_no = command_no();
        printf("com no %d,\n",com_no);
        switch (com_no){
            case 1:
                for(i = 0;i<radio_no;i++){
                    //关闭所有的射频
                    radio_disable_all(radios);
                    confirm_wireless();
                }
                 break;
            case 2:
                printf("recv1 : %s\n", recvbuf1);
                get_config(radios,recvbuf1);//获取命令中每个射频分配的channel和ssid
                for(i = 0;i<radio_no;i++){
                    if(radios[i].disabled == 0){
                        alloc_config(radios[i]);
                    }
                    else radio_disable(radios[i]);
                }
                confirm_wireless();
               
		 break;
                
            case 3:
                loads = getload(radios);
                radio_init(radios);
                alloc_config_all(radios);
                confirm_wireless();
                //wait!!! enough
                get_neighbor(loads,clientSocket,radios);
                printf("dicover is over,waiting for config");
		//修改，发送！
                //&&&&&get_neighbor(clientSocket,radios);
		break;
				//printf("stop\n");
            default:
                printf("command error\n");
                break;
        }
        recvbuf1[0] = '\0';
		//break;
        
    }
    //shutdown_net();
    //close(clientSocket);

    return 0;
 }
