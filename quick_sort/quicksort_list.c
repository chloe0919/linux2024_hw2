#include "list.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void q_free(struct list_head *l) {
  // Release all elements first, and then release the head.
  if (!l)
    return;
  element_t *n;
  element_t *node = list_first_entry(l, element_t, list);
  list_for_each_entry_safe(node, n, l, list) { free(node); }
  free(l);
}
struct list_head *q_new() {
  struct list_head *head = malloc(sizeof(struct list_head));
  if (head) {
    INIT_LIST_HEAD(head);
    return head;
  }
  return NULL;
}
int q_size(struct list_head *head) {
  if (!head || list_empty(head))
    return 0;
  int len = 0;
  struct list_head *node;
  list_for_each(node, head) len += 1;
  return len;
}
/* Verify if list is order */
static bool list_is_ordered(struct list_head *list) {
  if (!list)
    return false;
  bool first = true;
  int value;
  struct list_head *curr = list->next;
  while (curr != list) {
    element_t *entry = list_entry(curr, element_t, list);
    if (first) {
      value = entry->value;
      first = false;
    } else {
      if (entry->value < value)
        return false;
      value = entry->value;
    }
    curr = curr->next;
  }
  return true;
}
void rand_p(struct list_head **b, int len) {
  struct list_head *prev = (*b)->prev;
  prev->next = *b;
  struct list_head *curr0 = (*b);
  int rand_idx = rand() % (len - 1);
  while (rand_idx > 0) {
    curr0 = curr0->next;
    rand_idx--;
  }
  list_move(curr0, prev);
  if (curr0 != (*b))
    (*b) = (*b)->prev;
}
void quick_sort(struct list_head *head) {
  int n = q_size(head);
  int value;
  int i = 0;
  int max_level = 2 * n - 1;
  struct list_head *begin[max_level], *end[max_level];

  struct list_head *left = q_new(), *right = q_new(), *mid = q_new();
  struct list_head *result = q_new();
  begin[0] = head->next;
  end[0] = head->prev;
  int k = 0;

  while (i >= 0) {
    struct list_head *L = begin[i], *R = end[i];
    if (L != R) {
      struct list_head *l = L;
      int len = 1;
      while (l != R) {
        l = l->next;
        len += 1;
      }
      rand_p(&L, len);
      struct list_head *pivot = L;
      value = list_entry(pivot, element_t, list)->value;
      INIT_LIST_HEAD(left);
      INIT_LIST_HEAD(right);
      INIT_LIST_HEAD(mid);
      struct list_head *p = pivot->next;

      list_del_init(pivot);
      list_add(pivot, mid);

      while (p != R) {
        element_t *entry = list_entry(p, element_t, list);
        struct list_head *nex = p->next;
        if (entry->value > value) {
          list_del(p);
          list_add(p, right);
        } else {
          list_del(p);
          list_add(p, left);
        }
        p = nex;
      }
      element_t *entry = list_entry(R, element_t, list);
      if (entry->value > value) {
        list_del(p);
        list_add(p, right);
      } else {
        list_del(p);
        list_add(p, left);
      }

      begin[i] = left->next;
      end[i] = left->prev;
      begin[i + 1] = mid->next;
      end[i + 1] = mid->prev;
      begin[i + 2] = right->next;
      end[i + 2] = right->prev;
      i += 2;
    } else {
      if (L && !list_empty(L)) {
        list_del_init(L);
        list_add(L, result);
      }
      i--;
    }
    k++;
  }

  list_del_init(head);

  list_add(head, result);
  list_del(result);
  free(result);
  free(left);
  free(mid);
  free(right);
}
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
struct list_head *construct(struct list_head *list, int n) {
  element_t *node = malloc(sizeof(element_t));
  node->value = n;
  list_add(&node->list, list);
  return list;
}
int main(int argc, char **argv) {
  size_t count = 100000;

  int *test_arr = malloc(sizeof(int) * count);

  for (int i = 0; i < count; ++i)
    test_arr[i] = i;
  shuffle(test_arr, count);

  struct list_head *head = q_new();

  while (count--)
    construct(head, test_arr[count]);

  quick_sort(head);
  assert(list_is_ordered(head));
  free(head);
  free(test_arr);
  return 1;
}