#ifndef SERVER_H_   /* Include guard */
#define SERVER_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "utils.h"

#define BUFF_SIZE 4096

struct sockaddr_in init_server_socket_address(short, u_int, u_int);
int init_server(short sin_family, u_int s_addr, u_int sin_port, int socket_type, int protocol);
int handle_server_events(int server_socket);
void on_received_request(char* request);
int on_connection_closed(int event, fd_set* events, int* events_size);
int on_client_connecting(int* events, int* size);

#endif // FOO_H_
