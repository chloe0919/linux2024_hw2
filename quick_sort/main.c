#include "cpucycles.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define max(x, y) (x) > (y) ? (x) : (y)
typedef struct __node {
  struct __node *left, *right;
  struct __node *next;
  long value;
} node_t;
void list_add(node_t **list, node_t *node_t) {
  node_t->next = *list;
  *list = node_t;
}

node_t *list_tail(node_t **left) {
  while ((*left) && (*left)->next)
    left = &((*left)->next);
  return *left;
}

int list_length(node_t **left) {
  int n = 0;
  while (*left) {
    ++n;
    left = &((*left)->next);
  }
  return n;
}

node_t *list_construct(node_t *list, int n) {
  node_t *node = malloc(sizeof(node_t));
  node->next = list;
  node->value = n;
  return node;
}

void list_free(node_t **list) {
  node_t *node = (*list)->next;
  while (*list) {
    free(*list);
    *list = node;
    if (node)
      node = node->next;
  }
}

/* Verify if list is order */
static bool list_is_ordered(node_t *list) {
  bool first = true;
  int value;
  while (list) {
    if (first) {
      value = list->value;
      first = false;
    } else {
      if (list->value < value)
        return false;
      value = list->value;
    }
    list = list->next;
  }
  return true;
}

/* shuffle array, only work if n < RAND_MAX */
void shuffle(int *array, size_t n) {
  if (n <= 0)
    return;

  for (size_t i = 0; i < n - 1; i++) {
    size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
    int t = array[j];
    array[j] = array[i];
    array[i] = t;
  }
}
void rand_p(node_t **b) {
  node_t *prev = *b;
  node_t *curr = prev;
  int c = list_length(b);
  int rand_idx = rand() % c;

  while (rand_idx > 1) {
    prev = prev->next;
    rand_idx--;
  }
  if (rand_idx != 0) {
    curr = prev->next;
    prev->next = curr->next;
    list_add(b, curr);
  }
}

void quick_sort(node_t **list) {
  int n = list_length(list);
  int value;
  int i = 0;
  int max_level = 2 * n - 1;
  int count_i = 0;
  node_t *begin[max_level], *end[max_level];
  node_t *result = NULL, *left = NULL, *right = NULL;
  begin[0] = *list;
  end[0] = list_tail(list);

  while (i >= 0) {
    node_t *L = begin[i], *R = end[i];
    if (L != R) {
      if (L->next != R)
        rand_p(&L);
      node_t *pivot = L;
      value = pivot->value;
      // node_t *o = L;
      // while(o){
      //     printf("%ld,", o->value);
      //     o = o->next;
      // }

      // printf("pivot : %d\n", value);
      node_t *p = pivot->next;
      pivot->next = NULL;
      while (p) {
        node_t *n = p;
        p = p->next;
        list_add(n->value > value ? &right : &left, n);
      }

      begin[i] = left;
      end[i] = list_tail(&left);
      begin[i + 1] = pivot;
      end[i + 1] = pivot;
      begin[i + 2] = right;
      end[i + 2] = list_tail(&right);

      left = right = NULL;
      i += 2;
      count_i = max(count_i, i);
    } else {
      if (L)
        list_add(&result, L);
      i--;
    }
  }
  // printf("max_level : %d\n", max_level);
  printf("max lengh : %d\n", count_i + 1);
  *list = result;
}
int main(int argc, char **argv) {
  node_t *list = NULL;

  for (int i = 10; i <= 100000; i *= 10) {
    printf("%s%d\n", "count : ", i);
    size_t count = i;
    int *test_arr = malloc(sizeof(int) * count);

    for (int i = 0; i < count; i++)
      test_arr[i] = i;
    shuffle(test_arr, count);

    while (count--)
      list = list_construct(list, test_arr[count]);
    int64_t before = cpucycles();
    quick_sort(&list);
    int64_t after = cpucycles();

    assert(list_is_ordered(list));
    list_free(&list);

    free(test_arr);
    printf("%s%ld\n", "elasped cycles : ", (long int)after - (long int)before);
  }

  return 0;
}
