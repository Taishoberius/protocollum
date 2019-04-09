#include "server.h"

struct sockaddr_in init_server_socket_address(short sin_family, u_int sin_port, u_int s_addr) {
  struct sockaddr_in servaddr;
  servaddr.sin_family = sin_family;
  servaddr.sin_addr.s_addr = htons(sin_port);
  servaddr.sin_port = htons(s_addr);
  return servaddr;
}

int init_server(short sin_family, u_int s_addr, u_int sin_port, int socket_type, int protocol) {
  struct sockaddr_in servaddr = init_server_socket_address(sin_family, s_addr, sin_port);
  int sock_passive = socket(sin_family, socket_type, protocol);

  if(bind(sock_passive, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)))
      return -1;

  if (listen(sock_passive, 1))
      return -1;

  return sock_passive;
}

int handle_server_events(int server_socket) {
  fd_set events;
  int all_events[FD_SETSIZE];
  int size = 1;
  char request[BUFF_SIZE];
  int sock;
  int maxFd;
  int ret;

  all_events[0] = server_socket;

  while (1) {
    memset(request, '\0', BUFF_SIZE);
    FD_ZERO(&events);

    maxFd = all_events[0];
    for (int i = 0; i < size; i++) {
      FD_SET(all_events[i], &events);
      if (maxFd < all_events[i]) {
        maxFd = all_events[i];
      }
    }

    ret = select(maxFd + 1, &events, NULL, NULL, NULL);

    for (size_t i = 1; i < size; i++) {
      if (FD_ISSET(all_events[i], &events)) {
        if (read(all_events[i], request, BUFF_SIZE)) {
          on_received_request(request);
        } else {
          on_connection_closed(all_events[i], &events, &size);
        }
      }
    }

    if (FD_ISSET(all_events[0], &events)) {
      on_client_connecting(all_events, &size);
    }
  }

  return 0;
}

void on_received_request(char* request) {
  printf("%s\n", request);
}

int on_connection_closed(int event, fd_set* events, int* events_size) {
  close(event);
  FD_CLR(event, events);
  (*events_size)--;

  return (*events_size);
}

int on_client_connecting(int* events, int* size) {
  struct sockaddr cliaddr;
  socklen_t sa_len = sizeof(cliaddr);
  int socket;

  if ((socket = accept(events[0], (struct sockaddr*)&cliaddr, &sa_len)) == -1)
    return -1;

  events[(*size)] = socket;
  (*size)++;

  return socket;
}
