// TCP获取时间服务端程序
#include <arpa/inet.h>
#include <err.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

const int BUFFSIZE = 4096;

int main(int argc, char *argv[]) {
  int listenfd, connfd, nwrite;
  struct sockaddr_in servaddr;
  char buf[BUFFSIZE];

  time_t ticks;

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err(EXIT_FAILURE, "socket error");

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(1998);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    err(EXIT_FAILURE, "bind error");

  if (listen(listenfd, 10) < 0)
    err(EXIT_FAILURE, "listen error");

  for (;;) {
    connfd = accept(listenfd, NULL, NULL);
    ticks = time(NULL);
    snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
    if ((nwrite = send(connfd, buf, strlen(buf), 0)) < 0)
      err(EXIT_FAILURE, "write error");

    close(connfd);
  }

  return 0;
}