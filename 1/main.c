#include "stro.h"
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
  char buff[64];
} buff_data;

void *first_thread(void *arg) {
  buff_data *data = (buff_data *)arg;
  printf("Enter:");
  scanf("%63s", data->buff);

  if (strlen(data->buff) > 64) {
    printf("EROR:buffer overflow\n");
  }

  for (int i = 0; data->buff[i] != '\0'; i++) {
    if (!isdigit(data->buff[i])) {
      printf("ERROR: allowed only numbers\n");
      return NULL;
    }
  }
  sort_dcr(data->buff);
  
  return NULL;
}

void *second_thread(void *arg) {
  buff_data *data = (buff_data *)arg;
  printf("DATA:%s\n", data->buff);
  sum_str(data->buff);

  return NULL;
}

int main() {
  struct sockaddr_in addr, cl_addr;
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;

  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(8080);

if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("Error bind\n");
    return -1;
  } else {
    printf("Bind ok\n");
  }

  if (listen(sock, 1) == -1) {
    perror("Error listnening\n");
    return -1;
  } else {
    printf("Listening og\n");
  }

  // int client_fd = accept(client_fd, 0, 0);
  int client_fd = accept(sock, (struct sockaddr *)&cl_addr , (socklen_t *)&cl_addr);

  pthread_t thrf, thrs;
  // char buff[64];
  buff_data data;

  pthread_create(&thrf, NULL, first_thread, &data);
  pthread_join(thrf, NULL);

  pthread_create(&thrs, NULL, second_thread, &data);
  pthread_join(thrs, NULL);

  send(client_fd, data.buff, strlen(data.buff), 0);

  close(sock);
  close(client_fd);

  return 0;
}
