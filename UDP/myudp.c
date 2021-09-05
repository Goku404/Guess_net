


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



//创建一个TCP的套接字
//指定本地的 端口号 + ip
int create_udp_socket(short port, char* ip)
{
    int sock;
    int  ret;

    //创建一个套接字
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        perror("socket error");
        return -1;
    }

    //准备好本机的“网络地址”
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons( port );
    sa.sin_addr.s_addr = inet_addr(ip);

    //绑定一个“网络地址”
    ret = bind(sock, (struct sockaddr*) &sa, sizeof(sa));
    if (ret  == -1)
    {
        perror("bind error");
        close(sock);
        return -1;
    }

    return sock;
}



// int recv_data(int sock, unsigned char buf[], int len, struct sockaddr* src_addr,
//             socklen_t *addrlen)
// {
//     recvfrom(int sockfd, void *buf, size_t len, int flags,
//                 struct sockaddr *src_addr, socklen_t *addrlen);
// }
