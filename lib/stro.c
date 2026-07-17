#include "stro.h"

void sort_dcr(char *str) {
  size_t size = strlen(str);

  if (size < 2)
    return; // для безопасности

  for (size_t i = 0; i < size - 1; i++) {
    for (size_t j = 0; j < size - i - 1; j++) {
      if (str[j] < str[j + 1]) {
        int tmp = str[j];
        str[j] = str[j + 1];
        str[j + 1] = tmp;
      }
    }
  }

  return;
}

int sum_str(char *str) {
  int sum = 0;
  int num = 0;

  for (int i = 0; str[i] != '\0'; i++) {
    if (isdigit(str[i])) {
      num = num * 10 + (str[i] - '0');
    } else {
      sum += num;
      num = 0;
    }
  }
  sum += num;

  return sum;
}

bool analyse(char *str) {
  bool res = false;
  int sum = sum_str(str);

  if (strlen(str) > 2 && (sum % 32 == 0))
    res = true;

  return res;
}
