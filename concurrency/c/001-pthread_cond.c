#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct f_args {
  int n;
  pthread_cond_t *cv;
};

void *f(void *args) {
  struct f_args *a = (struct f_args *) args;
  struct drand48_data buffer;
  long r;

  srand48_r(time(NULL), &buffer);

  while (1) {
    lrand48_r(&buffer, &r);
    usleep((r % 6 + 1) * 1000000);

    pthread_cond_signal(a->cv);
  }

  return NULL;
}

int main(void) {
  pthread_t t;
  pthread_attr_t tattr;
  pthread_cond_t cv;
  pthread_condattr_t cvattr;
  pthread_mutex_t m;
  pthread_mutexattr_t mattr;

  pthread_attr_init(&tattr);

  pthread_condattr_init(&cvattr);
  pthread_cond_init(&cv, &cvattr);

  pthread_mutexattr_init(&mattr);
  pthread_mutex_init(&m, &mattr);

  struct f_args a = {1, &cv};

  pthread_create(&t, &tattr, &f, (void *)&a);
  pthread_detach(t);

  while (1) {
    pthread_cond_wait(&cv, &m);

    printf("Received from thread\n");
  }
}
