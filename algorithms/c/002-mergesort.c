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

void merge(int *a, int *b, int low, int middle, int high) {
  int il = low;
  int ir = middle + 1;
  int bi = low;

  while (il <= middle && ir <= high) {
    if (a[il] <= a[ir]) {
      b[bi++] = a[il++];
    } else {
      b[bi++] = a[ir++];
    }
  }

  while (il <= middle) {
    b[bi++] = a[il++];
  }

  while (ir <= high) {
    b[bi++] = a[ir++];
  }

  for (bi = low; bi <= high; bi++) {
    a[bi] = b[bi];
  }
}

void mergesort(int *a, int *b, int low, int high) {
  if (high - low > 0) {
    int middle = (low + high) / 2;
    mergesort(a, b, low, middle);
    mergesort(a, b, middle + 1, high);
    merge(a, b, low, middle, high);
  }
}

int main(void) {
  int n;
  int *a = NULL;
  int *b = NULL;

  srand(time(NULL));
  n = rand() % 41 + 10;
  a = calloc(n, sizeof(int));
  b = calloc(n, sizeof(int));

  for (int i = 0; i < n; i++) {
    a[i] = rand() % 101 + 1;
  }

  print_v(a, n);
  mergesort(a, b, 0, n - 1);
  print_v(a, n);

  free(a);
  free(b);

  return 0;
}
