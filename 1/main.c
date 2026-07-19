// #include "../lib/include/stro.h"
#include "stro.h"
#include <arpa/inet.h>
#include <bits/pthreadtypes.h>
#include <ctype.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
  char buff[64];
  int client_fd;
  bool ready;
} buff_data;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *first_thread(void *arg) {
  buff_data *data = (buff_data *)arg;
  char input[64];

  while (1) {
    printf("Enter:");
    scanf("%63s", input);

    bool ok = true;

    for (int i = 0; input[i] != '\0'; i++) {
      if (!isdigit((unsigned char)input[i])) {
        ok = false;
        break;
      }
    }

    if (!ok) {
      printf("ERROR: allowed only numbers\n");
      continue;
    }

    sort_dcr(input);

    pthread_mutex_lock(&m);

    while (data->ready) {
      pthread_cond_wait(&cond, &m);
    }

    strcpy(data->buff, input);
    data->ready = true;

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&m);
  }

  return NULL;
}

void *second_thread(void *arg) {
  buff_data *data = (buff_data *)arg;
  char copy[64];

  while (1) {
    pthread_mutex_lock(&m);
    while (!data->ready) {
      pthread_cond_wait(&cond, &m);
    }

    strcpy(copy, data->buff);

    memset(data->buff, 0, sizeof(data->buff));
    data->ready = false;

    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&m);

    printf("DATA:%s\n", copy);
    sum_str(copy);

    if (data->client_fd != -1) {
      if (send(data->client_fd, copy, strlen(copy), 0) == -1) {
        perror("send");
        close(data->client_fd);
      }
    }
  }

  return NULL;
}

int main() {
  struct sockaddr_in addr, cl_addr;
  pthread_t thrf, thrs;
  buff_data data;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;

  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  addr.sin_port = htons(8080);

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    perror("bind");
  }

  if (listen(sock, 1) == -1) {
    perror("listening");
  }

  socklen_t len = sizeof(cl_addr);
  int client_fd = accept(sock, (struct sockaddr *)&cl_addr, &len);

  data.client_fd = client_fd;
  data.ready = false;

  if (client_fd == -1)
    perror("accept");

  pthread_create(&thrf, NULL, first_thread, &data);
  pthread_create(&thrs, NULL, second_thread, &data);

  pthread_join(thrf, NULL);
  pthread_join(thrs, NULL);

  close(client_fd);
  close(sock);

  return 0;
}
