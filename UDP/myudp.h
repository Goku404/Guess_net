
#ifndef __MYUDP_H__
#define __MYUDP_H__

//创建一个TCP的套接字
//指定本地的 端口号 + ip
int create_udp_socket(short port, char* ip);

#endif