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
#include <sys/epoll.h>

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

int handle_client(int sock )
{
    struct sockaddr_in client;
    socklen_t addrlen = sizeof(client);

    unsigned char buf[33]={0};
    ret =   recvfrom(sock, buf,  32,  0,  (struct sockaddr *)&client, &addrlen);
    if (ret > 0)
    {
        printf("client  ip: %s  port :%u ", inet_ntoa(client.sin_addr) , ntohs( client.sin_port) );
                
        if (ret >= 32)
        {
            //
            if (strncmp(buf,"name:", 5) == 0)
            {
                unsigned char name[32] = {0};
                strcpy(name, buf+5);
                int id = add_node(l, name);

                printf("name: %s id: %d\n", name, id);

                unsigned char resp[4];
                resp[0] = id & 0xff;
                resp[1] = (id >> 8) & 0xff;
                resp[2] = (id >> 16) & 0xff;
                resp[3] = (id >> 24) & 0xff;
                sendto(sock, resp, 4, 0, (struct sockaddr*)&client, sizeof(client));
            }
        }
        else if (ret >= 8)
        {
            int id = (buf[0] )  | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
            int x = (buf[4] )  | (buf[5] << 8) | (buf[6] << 16) | (buf[7] << 24);

            int ret =  add_record(l, id, x);

            printf("id: %d  x: %d\n", id,x);
            unsigned char resp[4];
            resp[0] = ret & 0xff;
            resp[1] = (ret >> 8) & 0xff;
            resp[2] = (ret >> 16) & 0xff;
            resp[3] = (ret >> 24) & 0xff;
            sendto(sock, resp, 4, 0, (struct sockaddr*)&client, sizeof(client));
        }
    }

    return 0;
}


// udp_server ip  port
int main(int argc, char *argv[])
{
    int ret;
    int sock;

    //注册信号处理函数
    signal(SIGINT, sig_handler);

    sock = create_udp_socket( atoi(argv[2]), argv[1]);
    if (sock == -1)
    {
        printf("failed to create_udp_socket\n");
        return -1;
    }


    guess_digtal_t*l = guess_digtal_start();

    int epfd = epoll_create(1); //创建一个EPOLL实例，“监听文件描述符集合”

    //往EPOLL实例中，添加/删除/修改  监听事件
    struct epoll_event ev;
    ev.events = EPOLLIN; //监听读的事件
    ev.data.fd = sock; //私人数据，保存“套接字描述符”
    epoll_ctl(epfd, EPOLL_ADD, sock, &ev); //

    while (!terminate)
    {
        int n;
        struct epoll_event evs[100];
        n = epoll_wait(epfd, evs, 100, 2000);
        if (n == 0)
        {
            continue;
        }
        else if (n == -1)
        {
            perror("epoll_wait");
            continue;
        }

        for (i  = 0 ; i < n; i++)
        {

            if (evs[i].events & EPOLLIN)
            {
                handle_client(evs[i].data.fd);
            }
        }
    }

    destroy_list(l);


    return 0;
}

