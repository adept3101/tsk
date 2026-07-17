#include "stro.h"
#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char buff[64];
} buff_data;

void *first_thread(void *arg) {
  buff_data *data = (buff_data *)arg;
  printf("Enter:");
  scanf("%s", data->buff);

  if (strlen(data->buff) > 64) {
    printf("EROR:buffer overflow\n");
  }

  for (int i = 0; data->buff[i] != '\0'; i++) {
    if (!isdigit(data->buff[i])) {
      printf("ERROR: allowed only numbers\n");
      break;
    }
  }
  sort_dcr(data->buff);
  
  return NULL;
}

int main() {
  pthread_t thrf, thrs;
  // char buff[64];
  buff_data data;

  pthread_create(&thrf, NULL, first_thread, &data);

  return 0;
}
