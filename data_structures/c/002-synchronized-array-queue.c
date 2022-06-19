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

struct queue {
  int *v;
  int capacity;
  int elements;
  pthread_cond_t has_element;
  pthread_mutex_t m, m_has_element;
};

void print_queue(struct queue *q) {
  printf("[");
  for (int i = 0; i < q->capacity; i++) {
    printf(i < q->capacity - 1 ? "%d, " : "%d", (q->v)[i]);
  }
  printf("]\n");
}

void queue_init(struct queue *q) {
  pthread_mutexattr_t m_attr, m_has_element_attr;
  pthread_condattr_t cv_attr;

  pthread_condattr_init(&cv_attr);
  pthread_cond_init(&(q->has_element), &cv_attr);
  pthread_mutexattr_init(&m_attr);
  pthread_mutex_init(&(q->m), &m_attr);
  pthread_mutexattr_init(&m_has_element_attr);
  pthread_mutex_init(&(q->m_has_element), &m_has_element_attr);
  q->capacity = 8;
  q->elements = 0;
  q->v = (int *)calloc(q->capacity, sizeof(int));
}

void queue_destroy(struct queue *q) {
  free(q->v);
  pthread_mutex_destroy(&(q->m_has_element));
  pthread_mutex_destroy(&(q->m));
  pthread_cond_destroy(&(q->has_element));
}

void queue_insert(struct queue *q, int n) {
  pthread_mutex_lock(&(q->m));
  if (q->elements + 1 >= q->capacity) {
    q->capacity *= 2;
    q->v = (int *)realloc(q->v, q->capacity * sizeof(int));
  }
  (q->v)[q->elements] = n;
  q->elements += 1;
  pthread_cond_broadcast(&(q->has_element));
  pthread_mutex_unlock(&(q->m));
}

int queue_front(struct queue *q) {
  int r;
  if (q->elements > 0) {
    r = (q->v)[0];
  } else {
    r = INT_MIN;
  }

  return r;
}

int queue_remove(struct queue *q) {
  if (q->elements == 0) {
    pthread_cond_wait(&(q->has_element), &(q->m_has_element));
  }
  int r = queue_front(q);

  pthread_mutex_lock(&(q->m));
  shift_v(q->v, q->capacity, -1);
  q->elements -= 1;
  pthread_mutex_unlock(&(q->m));

  return r;
}

int main(void) {
  struct queue q;

  queue_init(&q);

  queue_insert(&q, 8);
  queue_insert(&q, 7);
  queue_insert(&q, 6);
  queue_insert(&q, 5);
  queue_insert(&q, 4);
  queue_insert(&q, 3);
  queue_insert(&q, 2);
  queue_insert(&q, 1);
  print_queue(&q);

  queue_insert(&q, 0);
  print_queue(&q);

  printf("%d\n", queue_front(&q));
  queue_remove(&q);
  print_queue(&q);

  queue_destroy(&q);

  return 0;
}
