#include<stdio.h>  
#include<stdlib.h>  
#include<netinet/in.h>  
#include<sys/socket.h>  
#include<arpa/inet.h>  
#include<string.h>  
#include<unistd.h>  
#define BUFFER_SIZE 1024  

char recv_msg[BUFFER_SIZE];  
char mainMsg[BUFFER_SIZE - 20];  
char sendMsg[BUFFER_SIZE];

void MakeFrame();
  
int main(int argc, const char * argv[])  
{  
    struct sockaddr_in server_addr;  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(11332);  
    /* 本地IP转换成网络IP */
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    bzero(&(server_addr.sin_zero), 8);  
  
    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);  
    if(server_sock_fd == -1)  
    {  
        perror("socket error");  
        return 1;  
    }  
  
    if(connect(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) == 0)  
    {  
        fd_set client_fd_set;  
        struct timeval tv;  
    
        while(1)  
        {  
            tv.tv_sec = 20;  
            tv.tv_usec = 0;  

            /* 把fd_set类型变量的所有位设置为0 */
            FD_ZERO(&client_fd_set);  
            FD_SET(STDIN_FILENO, &client_fd_set);  
            FD_SET(server_sock_fd, &client_fd_set);  
    
            /* select函数监听所有的句柄 */
            select(server_sock_fd + 1, &client_fd_set, NULL, NULL, &tv);  

            /* 判断client_fd_set是否被建立成功 */
            if(FD_ISSET(STDIN_FILENO, &client_fd_set))  
            {  
                bzero(mainMsg, BUFFER_SIZE);  
                printf("me:");
                fgets(mainMsg, BUFFER_SIZE, stdin);  

                MakeFrame();
                if(send(server_sock_fd, sendMsg, BUFFER_SIZE, 0) == -1)  
                {  
                    perror("发送消息出错!\n");  
                }  
            }  
            if(FD_ISSET(server_sock_fd, &client_fd_set))  
            {  
                bzero(recv_msg, BUFFER_SIZE);  
                long byte_num = recv(server_sock_fd, recv_msg, BUFFER_SIZE, 0);  
                if(byte_num > 0)  
                {  
                    if(byte_num > BUFFER_SIZE)  
                    {  
                        byte_num = BUFFER_SIZE - 1;  
                    }  

                    recv_msg[byte_num] = '\0';  
                    printf("\t\trecv:%s", recv_msg);    
                }  
                else if(byte_num < 0)  
                {  
                    printf("接受服务器消息出错!\n");  
                }  
                else  
                {  
                    printf("服务器端退出!\n");  
                    exit(0);  
                }  
            }  
        }    
    }  
    
    return 0;  
} 

/* 打包发送数据帧 */
void MakeFrame()
{
    char sendClient[10] = "b003";
    char recvClient[10] = "a001";

    memcpy(&sendMsg[0], sendClient, 10);
    memcpy(&sendMsg[10], recvClient, 10);
    memcpy(&sendMsg[20], mainMsg, sizeof(mainMsg));    
}