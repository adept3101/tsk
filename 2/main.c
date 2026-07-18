#include "stro.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

int main() {
  char buff[64];
  struct sockaddr_in addr;
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  memset(&addr, 0, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(8080);

  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("Error connect\n");
    return -1;
  } else {
    printf("Connect ok\n");
  }
  
  int rb = recv(sock, buff, sizeof(buff) - 1, 0);

  if (rb > 0) {
    buff[rb] = '\0';
    printf("%s\n", buff);
  }

  close(sock);

  return 0;
}
