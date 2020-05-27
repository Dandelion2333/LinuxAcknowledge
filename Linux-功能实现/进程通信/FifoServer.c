#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>

#define     FIFO_NAME   "FifoPipe"

int main(void)
{
    int result = 0;
    int fifoFd = 0;
    char buffer[32] = {0};
    int buffLen = 0;

    /* 1、创建命名管道 */
    unlink(FIFO_NAME);          // 删除可能存在遗留的命名管道
    result = mkfifo(FIFO_NAME, 0777);
    if(result != 0)
    {
        perror("create fifo error\n");
        return 1;    
    }

    /* 2、打开命名管道 */
    fifoFd = open(FIFO_NAME, O_RDONLY);
    if(fifoFd < 0)
    {
        perror("open fifo error\n");
        return 1;
    }

	while( memcmp( buffer, "close", 5 ) != 0 )
	{
		memset( buffer, 0, 32 );
		buffLen = read( fifoFd, buffer, 31 );
		buffer[buffLen] = '\0';
		printf( "read:%s\n", buffer );
	}
	

    close(fifoFd);
    unlink(FIFO_NAME);

    return 0;
}