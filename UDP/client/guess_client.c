#include <sys/types.h>			/* See NOTES */
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "myudp.h"
#include "game.h"
#include "guess_digtal.h"


//全局变量，表示进程是否退出
int terminate =FALSE;

//信号处理函数
void sig_handler(int sig)
{
    switch(sig)
    {
        case SIGINT:
            terminate = TRUE;
            break;
        default:
            break;
    }
}

// udp_server ip  port
int main(int argc, char *argv[])
{
    int ret;
    int sock;

    //注册信号处理函数
    signal(SIGINT, sig_handler);

     //创建一个套接字
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket error");
        return -1;
    }

    
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons( atoi(argv[2]) );
    sa.sin_addr.s_addr = inet_addr(argv[1]);


	unsigned char buf[32] ={0};
	strcpy(buf,"name:zhou");
	sendto(sock, buf, 32, 0, (struct sockaddr*)&sa, sizeof(sa));


	unsigned char id[8];
	ret =   recvfrom(sock, id,  4,  0,  NULL, NULL);
	if (ret == 4)
	{
		int game_id = id[0] | (id[1] << 8) | (id[2] << 16) | (id[3] << 24);
		printf("game_id : %d\n", game_id);
	}

	while (!terminate)
	{
		int x;
		scanf("%d", &x);

		unsigned char digtal[4];
		id[4] = x & 0xff;
		id[5] = x >> 8;
		id[6] = x >> 16;
		id[7] = x >> 24;
		
		sendto(sock, id, 8, 0, (struct sockaddr*)&sa, sizeof(sa));

		unsigned char resp[4];
		ret =	recvfrom(sock, resp,	4,	0,	NULL, NULL);
		if (ret == 4)
		{
			int a = (resp[0]) | (resp[1] << 8) | (resp[2]  << 16) | (resp[3] << 24);
			printf("%d\n",a);
			if (a == 0)
			{
				printf("OK\n");
				break;
			}
		}

	}
}
