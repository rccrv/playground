#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void shift_v(int *v, int n, int p) {
  if (p > 0) {
    int s = p;
    memmove(v + s, v, (n - s) * sizeof(int));
    memset(v, 0, s * sizeof(int));
  } else if (p < 0) {
    int s = -p;
    memmove(v, v + s, (n - s) * sizeof(int));
    memset(v + (n - s), 0, s * sizeof(int));
  }
}

struct deque {
  int *v;
  int capacity;
  int elements;
  pthread_cond_t has_element;
  pthread_mutex_t m, m_has_element;
};

void print_deque(struct deque *d) {
  printf("[");
  for (int i = 0; i < d->capacity; i++) {
    printf(i < d->capacity - 1 ? "%d, " : "%d", (d->v)[i]);
  }
  printf("]\n");
}

void deque_init(struct deque *d) {
  pthread_mutexattr_t m_attr, m_has_element_attr;
  pthread_condattr_t cv_attr;

  pthread_condattr_init(&cv_attr);
  pthread_cond_init(&(d->has_element), &cv_attr);
  pthread_mutexattr_init(&m_attr);
  pthread_mutex_init(&(d->m), &m_attr);
  pthread_mutexattr_init(&m_has_element_attr);
  pthread_mutex_init(&(d->m_has_element), &m_has_element_attr);
  d->capacity = 8;
  d->elements = 0;
  d->v = (int *)calloc(d->capacity, sizeof(int));
}

void deque_destroy(struct deque *d) {
  free(d->v);
  pthread_mutex_destroy(&(d->m_has_element));
  pthread_mutex_destroy(&(d->m));
  pthread_cond_destroy(&(d->has_element));
}

void deque_insert_front(struct deque *d, int n) {
  pthread_mutex_lock(&(d->m));
  if (d->elements + 1 >= d->capacity) {
    d->capacity *= 2;
    d->v = (int *)realloc(d->v, d->capacity * sizeof(int));
  }
  shift_v(d->v, d->capacity, 1);
  (d->v)[0] = n;
  d->elements += 1;

  pthread_cond_broadcast(&(d->has_element));
  pthread_mutex_unlock(&(d->m));
}

void deque_insert_back(struct deque *d, int n) {
  pthread_mutex_lock(&(d->m));
  if (d->elements + 1 >= d->capacity) {
    d->capacity *= 2;
    d->v = (int *)realloc(d->v, d->capacity * sizeof(int));
  }
  (d->v)[d->elements] = n;
  d->elements += 1;
  pthread_cond_broadcast(&(d->has_element));
  pthread_mutex_unlock(&(d->m));
}

int deque_front(struct deque *d) {
  int r;
  if (d->elements > 0) {
    r = (d->v)[0];
  } else {
    r = INT_MIN;
  }

  return r;
}

int deque_back(struct deque *d) {
  int r;
  if (d->elements > 0) {
    r = (d->v)[d->elements - 1];
  } else {
    r = INT_MIN;
  }

  return r;
}

int deque_remove_front(struct deque *d) {
  if (d->elements == 0) {
    pthread_cond_wait(&(d->has_element), &(d->m_has_element));
  }
  pthread_mutex_lock(&(d->m));
  int r = deque_front(d);

  shift_v(d->v, d->capacity, -1);
  d->elements -= 1;
  pthread_mutex_unlock(&(d->m));

  return r;
}

int deque_remove_back(struct deque *d) {
  if (d->elements == 0) {
    pthread_cond_wait(&(d->has_element), &(d->m_has_element));
  }
  pthread_mutex_lock(&(d->m));
  int r = deque_back(d);

  (d->v)[d->elements - 1] = 0;
  d->elements -= 1;
  pthread_mutex_unlock(&(d->m));

  return r;
}

int main(void) {
  struct deque d;

  deque_init(&d);

  printf("Insert back:\n");
  deque_insert_back(&d, 8);
  deque_insert_back(&d, 7);
  deque_insert_back(&d, 6);
  deque_insert_back(&d, 5);
  deque_insert_back(&d, 4);
  deque_insert_back(&d, 3);
  deque_insert_back(&d, 2);
  print_deque(&d);
  deque_insert_back(&d, 1);
  print_deque(&d);

  deque_insert_back(&d, 15);
  print_deque(&d);

  printf("Remove front:\n");
  printf("%d\n", deque_front(&d));
  deque_remove_front(&d);
  print_deque(&d);

  deque_destroy(&d);

  deque_init(&d);

  printf("Insert front:\n");
  deque_insert_front(&d, 1);
  deque_insert_front(&d, 2);
  deque_insert_front(&d, 3);
  deque_insert_front(&d, 4);
  deque_insert_front(&d, 5);
  deque_insert_front(&d, 6);
  deque_insert_front(&d, 7);
  print_deque(&d);
  deque_insert_front(&d, 8);
  print_deque(&d);

  printf("Remove back:\n");
  printf("%d\n", deque_back(&d));
  deque_remove_back(&d);
  print_deque(&d);

  deque_destroy(&d);

  return 0;
}
