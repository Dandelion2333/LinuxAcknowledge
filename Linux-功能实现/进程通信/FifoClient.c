#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#define     FIFO_NAME   "FifoPipe"

int main()
{
    int result = 0;
    char buff[64] = {0};
	int fifoFd = 0;
    int i;

    /* 1、判断管道是否存在 */
    result = access(FIFO_NAME, F_OK);
    if(result == -1)
    {
        perror("pipe not exist\n");
        return 0;
    }

    /* 2、打开管道 */
    fifoFd = open(FIFO_NAME, O_WRONLY);
    if(fifoFd < 0)
    {
        perror("open pipe fail\n");
        return 0;
    }

    /* 3、往管道中写入数据 */
    while( memcmp(buff, "close", 5) != 0)
    {
        printf("please input:");
        scanf("%s", buff);
        write(fifoFd, buff, strlen(buff));
        printf("buff data:%s\n", buff);
    }

    close(fifoFd);
    unlink(FIFO_NAME);

    return 0;
}