#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_v(int *p, int n) {
  printf("[");
  for (int i = 0; i < n; i++) {
    printf(i < n - 1 ? "%d, " : "%d", p[i]);
  }
  printf("]\n");
}

int part(int *a, int low, int high) {
  int pivot = high;
  int aux = 0;
  int j = low;

  for (int i = low; i < high; i++) {
    if (a[i] < a[pivot]) {
      aux = a[i];
      a[i] = a[j];
      a[j] = aux;
      j++;
    }
  }

  aux = a[j];
  a[j] = a[pivot];
  a[pivot] = aux;

  return j;
}

void quicksort(int *a, int low, int high) {
  if (high - low > 0) {
    int j = part(a, low, high);
    quicksort(a, low, j - 1);
    quicksort(a, j + 1, high);
  }
}

int main(void) {
  int n;
  int *a = NULL;

  srand(time(NULL));
  n = rand() % 41 + 10;
  a = calloc(n, sizeof(int));

  for (int i = 0; i < n; i++) {
    a[i] = rand() % 101 + 1;
  }

  print_v(a, n);
  quicksort(a, 0, n - 1);
  print_v(a, n);

  free(a);

  return 0;
}
