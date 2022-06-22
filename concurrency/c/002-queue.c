#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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
  pthread_mutex_lock(&(q->m));
  int r = queue_front(q);

  shift_v(q->v, q->capacity, -1);
  q->elements -= 1;
  pthread_mutex_unlock(&(q->m));

  return r;
}

struct f_args {
  int n;
  struct queue *q;
};

void *f(void *args) {
  struct f_args *a = (struct f_args *)args;
  struct drand48_data buffer;
  long r;

  srand48_r(time(NULL) + a->n, &buffer);

  while (1) {
    lrand48_r(&buffer, &r);
    usleep((r % 6 + 1) * 1000000);

    queue_insert(a->q, a->n);
  }
}

int main(void) {
  pthread_t *t;
  pthread_attr_t *t_attr;
  struct f_args *a;
  struct queue q;
  int n;
  int r;

  queue_init(&q);

  srand(time(NULL));

  n = rand() % 5 + 3;

  printf("Allocating %d threads\n", n);

  t = (pthread_t *)calloc(n, sizeof(pthread_t));
  t_attr = (pthread_attr_t *)calloc(n, sizeof(pthread_attr_t));
  a = (struct f_args *)calloc(n, sizeof(struct f_args));

  for (int i = 0; i < n; i++) {
    a[i] = (struct f_args){i + 1, &q};
    pthread_create(&t[i], &t_attr[i], &f, &a[i]);
    pthread_detach(t[i]);
  }

  while (1) {
    r = queue_remove(&q);
    printf("Received from %d\n", r);
  }

  return 0;
}
