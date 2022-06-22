#include <stdio.h>
#include <stdlib.h>

struct node {
  int d;
  struct node *p;
  struct node *n;
};

struct list {
  int nelem;
  struct node *l;
  struct node *first;
  struct node *last;
};

struct list *list_initialize() {
  struct list *r = (struct list *)calloc(1, sizeof(struct list));

  return r;
}

void print_list(struct list *l) {
  struct node *p = l->l;

  printf("[");
  for (int i = 0; i < l->nelem; i++) {
    printf(i < l->nelem - 1 ? "%d, " : "%d", p->d);
    p = p->n;
  }
  printf("]\n");
}

void list_insert_front(struct list *l, int d) {
  struct node *new = (struct node *)calloc(1, sizeof(struct node));
  if (l->nelem > 0) {
    struct node *first = l->first;
    struct node *previous = first->p;

    new->d = d;

    new->n = first;
    first->p = new;
    new->p = previous;
    previous->n = new;
    l->first = new;
    l->l = new;

    l->nelem += 1;
  } else {
    new->d = d;
    new->n = new;
    new->p = new;

    l->l = new;
    l->first = new;
    l->last = new;

    l->nelem += 1;
  }
}

void list_insert_back(struct list *l, int d) {
  struct node *new = (struct node *)calloc(1, sizeof(struct node));
  if (l->nelem > 0) {
    struct node *last = l->last;
    struct node *next = last->n;

    new->d = d;

    new->p = last;
    last->n = new;
    new->n = next;
    next->p = new;
    l->last = new;

    l->nelem += 1;
  } else {
    new->d = d;

    new->n = new;
    new->p = new;
    l->l = new;
    l->first = new;
    l->last = new;

    l->nelem += 1;
  }
}

void list_remove_front(struct list *l) {
  if (l->nelem > 1) {
    struct node *previous = l->last;
    struct node *delete = l->first;
    struct node *next = delete->n;

    previous->n = delete->n;
    next->p = delete->p;
    l->first = next;
    l->l = l->first;

    free(delete);
    l->nelem -= 1;
  } else if (l->nelem == 1) {
    struct node *delete = l->l;

    l->l = NULL;
    l->first = NULL;
    l->last = NULL;

    free(delete);
    l->nelem -= 1;
  }
}

void list_remove_back(struct list *l) {
  if (l->nelem > 1) {
    struct node *next = l->first;
    struct node *delete = l->last;
    struct node *previous = delete->p;

    previous->n = delete->n;
    next->p = delete->p;
    l->last = previous;

    free(delete);
    l->nelem -= 1;
  } else if (l->nelem == 1) {
    struct node *delete = l->l;

    l->l = NULL;
    l->first = NULL;
    l->last = NULL;

    free(delete);
    l->nelem -= 1;
  }
}

struct node *list_find(struct list *l, int d) {
  struct node *r = l->l;

  for (int i = 0; i < l->nelem - 1; i++) {
    if (d == r->d) {
      return r;
    }
    r = r->n;
  }

  r = r->d == d ? r : NULL;

  return r;
}

int main(void) {
  int n;
  struct list *l;

  l = list_initialize();

  list_insert_back(l, 10);
  print_list(l); // [10]
  list_remove_back(l);
  print_list(l);       // []
  list_remove_back(l); // Should not segfault
  list_insert_front(l, 10);
  print_list(l); // [10]
  list_remove_front(l);
  print_list(l);        // []
  list_remove_front(l); // Should not segfault
  list_insert_back(l, 10);
  for (int i = 0; i < 10; i++) {
    list_insert_front(l, 20 - (i + 1));
  }
  print_list(l); // [10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 10]
  list_remove_front(l);
  print_list(l); // [11, 12, 13, 14, 15, 16, 17, 18, 19, 10]
  list_remove_back(l);
  print_list(l); // [11, 12, 13, 14, 15, 16, 17, 18, 19]
  list_insert_back(l, 100);
  print_list(l); // [11, 12, 13, 14, 15, 16, 17, 18, 19, 100]
  struct node *f = list_find(l, 100);
  if (f) {
    printf("%d\n", f->d);
  }

  return 0;
}
