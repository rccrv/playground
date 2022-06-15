#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct node {
  int d;
  struct node *n;
};

struct list {
  int nelem;
  struct node *l;
};

struct list *list_initialize() {
  struct list *r = (struct list *) calloc(1, sizeof(struct list));

  return r;
}

void print_list(struct list *l) {
  struct node *p = l->l;

  printf("[");
  while (p) {
    printf(p->n ? "%d, " : "%d", p->d);
    p = p->n;
  }
  printf("]\n");
}

void list_insert_front(struct list *l, int d) {
  struct node *new = (struct node *) calloc(1, sizeof(struct node));;

  new->d = d;
  new->n = l->l;
  l->l = new;
  l->nelem += 1;
}

void list_insert_back(struct list *l, int d) {
  struct node *new = (struct node *) calloc(1, sizeof(struct node));

  new->d = d;
  new->n = NULL;

  if (l->l) {
    struct node *second_to_last;
    second_to_last = l->l;
    while (second_to_last->n) {
      second_to_last = second_to_last->n;
    }
    second_to_last->n = new;
  } else {
    l->l = new;
  }
  l->nelem += 1;
}

void list_remove_front(struct list *l) {
  if (l->l) {
    struct node *delete = l->l;

    l->l = delete->n;

    free(delete);
    l->nelem-= 1;
  }
}

void list_remove_back(struct list *l) {
  if (l->l) {
    struct node *second_to_last, *last, *delete;
    second_to_last = l->l;
    last = second_to_last->n;
    if (last) {
      while (second_to_last->n && last->n) {
        second_to_last = second_to_last->n;
        last = second_to_last->n;
      }
      delete = last;
      second_to_last->n = NULL;
      free(delete);
    } else {
      delete = second_to_last;
      free(delete);
      l->l = NULL;
    }
    l->nelem-= 1;
  }
}

struct node *list_find(struct list *l, int d) {
  struct node *r = l->l;

  while (r) {
    if (d == r->d) {
      break;
    }
    r = r->n;
  }

  return r;
}

int main(void) {
  int n;
  struct list *l;

  srand(time(NULL));
  l = list_initialize();

  list_insert_back(l, 10);
  print_list(l);
  list_remove_back(l);
  print_list(l);
  list_remove_back(l); // Should not segfault
  list_insert_front(l, 10);
  print_list(l);
  list_remove_front(l);
  print_list(l);
  list_remove_front(l); // Should not segfault
  list_insert_back(l, 10);
  for (int i = 0; i < 10; i++) {
    list_insert_front(l, rand() % 101 + 1);
  }
  print_list(l);
  list_remove_front(l);
  print_list(l);
  list_remove_back(l);
  print_list(l);
  list_insert_back(l, 100);
  print_list(l);
  struct node *f = list_find(l, 100);
  if (f) {
    printf("%d\n", f->d);
  }

  return 0;
}
