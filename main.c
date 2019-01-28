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
#define SERVER_PORT 6666
/*
连接到服务器后，会不停循环，等待输入，
输入quit后，断开与服务器的连接
*/
int main()
{
	//客户端只需要一个套接字文件描述符，用于和服务器通信
	int clientSocket;
	//描述服务器的socket
	struct sockaddr_in serverAddr;
	char sendbuf[200];
	char recvbuf[200];
	int iDataNum;
    int com_no = 0;
    radio_no = 0;
    int i = 0;
    
	if((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
    	perror("socket");
    	return 1;
	}
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    //指定服务器端的ip，本地测试：127.0.0.1
    //inet_addr()函数，将点分十进制IP转换成网络字节序IP
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
    	perror("connect");
    	return 1;
    }
    printf("连接到主机...\n");
        	printf("发送消息:");
    	scanf("%s", sendbuf);
        //get the mac addr of radio0 as node ID
   		printf("\n");
        get_nodeid();
    	send(clientSocket, sendbuf, strlen(sendbuf), 0);
    
    struct radio_type * radios = NULL;
    radios_inform_init(radios);
    
    while(1)

    {
    	printf("发送消息:");
    	scanf("%s", sendbuf);
        //get the mac addr of radio0 as node ID
   		printf("\n");
    	send(clientSocket, sendbuf, strlen(sendbuf), 0);
        
        radios_inform_init(radios);
   		if(strcmp(sendbuf, "quit") == 0)
    		break;
    	printf("读取消息:");
    	recvbuf[0] = '\0';
    	iDataNum = recv(clientSocket, recvbuf, 200, 0);
    	recvbuf[iDataNum] = '\0';
    	printf("%s\n", recvbuf);
        com_no = decode_command(recvbuf);
        //char com_no = command_no();
        switch (com_no){
            case 1:
            for(i = 0;i<radio_no;i++){
                radio_disable(radios[i]);
            }
                //break;
            case 2:
                get_channel_ssid(radios,recvbuf);
                for(i = 0;i<radio_no;i++){
                    alloc_channel_ssid(radios[i]);
                }
                
                //uci set wireless.@wifi-device[0].channel=6    //设置无线信道为6  
                //uci set wireless.@wifi-iface[0].mode=ap    //设置无线模式为ap  
                //uci set wireless.@wifi-iface[0].ssid=[自己设置SSID]    //设置无线SSID 
                //break;
            case 3:
                get_neighbor(clientSocket,radios);
            default:
                printf("command error\n");
                break;
        }
    }
    shutdown_net();
    close(clientSocket);
    return 0;
 }
