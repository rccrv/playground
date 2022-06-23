#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void print_v(int *p, int n) {
  printf("[");
  for (int i = 0; i < n; i++) {
    printf(i < n - 1 ? "%d, " : "%d", p[i]);
  }
  printf("]\n");
}

int part(int *a, int low, int high) {
  int j = low;
  int aux = 0;
  int pivot = high;

  for (int i = low; i < high; i++) {
    if (a[i] < a[pivot]) {
      aux = a[i];
      a[i] = a[j];
      a[j] = aux;
      j += 1;
    }
  }

  aux = a[pivot];
  a[pivot] = a[j];
  a[j] = aux;

  return j;
}

void quicksort(int *a, int low, int high) {
  if (high - low > 0) {
    int j = part(a, low, high);
    quicksort(a, low, j - 1);
    quicksort(a, j + 1, high);
  }
}

int quickselect(int *a, int low, int high, int k) {
  if (low == high) {
    return a[low];
  } else {
    int j = part(a, low, high);
    if (k < j) {
      return quickselect(a, low, j - 1, k);
    } else if (k > j) {
      return quickselect(a, j + 1, high, k);
    } else {
      return a[j];
    }
  }
}

int main(void) {
  int n;
  int *a = NULL;

  srand(time(NULL));
  n = rand() % 11 + 10;
  a = calloc(n, sizeof(int));

  for (int i = 0; i < n; i++) {
    a[i] = rand() % 101 + 1;
  }

  print_v(a, n);
  int k = rand() % n;
  int s = quickselect(a, 0, n - 1, k);
  // C arrays are 0-indexed
  printf("%d-th smallest: %d\n", k + 1, s);
  quicksort(a, 0, n - 1);
  print_v(a, n);

  free(a);

  return 0;
}
