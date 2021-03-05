/* File Name: client.c */  
  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<errno.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<arpa/inet.h>
#include<netinet/in.h>  
#include<unistd.h>
  
#define MAXLINE 4096  
#define PROT    8002
#define IP      "127.0.0.1"
  
  
int main(int argc, char** argv)  
{  
    int sockfd, n,rec_len;  
    char recvline[4096], sendline[4096];  
    char buf[MAXLINE];  
    struct sockaddr_in servaddr;  
    char exitBuff[1024] = "exit\n";
  
    /* 创建一个socket套接字 */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {  
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
        exit(0);  
    }  
  
    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(PROT);

    /* 把本地IP地址转换成网络地址 */
    if(inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0)
    {  
        printf("inet_pton error for %s\n", IP);  
        exit(0);  
    }  
  
    /* 与服务器建立连接 */
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {  
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
        exit(0);  
    }  

    while(strcmp(sendline, exitBuff) != 0)
    {
        printf("input send data:");  
        fgets(sendline, 4096, stdin);  
        /* send函数可以把数据发送到服务器 */
        if( send(sockfd, sendline, strlen(sendline), 0) < 0)  
        {  
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
        }  
        else
        {
            printf("client send: %s", sendline);  
        }
        /* recv函数可以接受来自服务器的进程发过来的数据 */
        if((rec_len = recv(sockfd, buf, MAXLINE,0)) == -1) 
        {  
            perror("recv error");  
        }  

        buf[rec_len]  = '\0';  
        printf("client recv: %s\n",buf);  
    }

    close(sockfd);  
    exit(0);  
}  