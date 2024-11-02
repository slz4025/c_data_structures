#include <assert.h>
#include <stdio.h>

#include "../include/dict.h"
#include "../include/item.h"
#include "../include/linked_list.h"
#include "../include/list.h"
#include "../include/list_extended.h"
#include "../include/memory.h"
#include "../include/heap.h"

// A Fibonnaci heap_t has the following optimal running-times:
// - Peek-min: O(1)
// - Insert: O(1)
// - Decrease-key: O(1) (amortized)
// - Meld: O(1)
// - Pop-min: O(lgN) (amortized)
//
// PROPERTIES
//
// 0. A Fibonnaci heap_t is a forest.
// 1. Each node is the minimum among its descendents,
// 2. A node is cut from its parent and moved to the forest if its key
//    is less than its parent's, i.e. through decrease-key.
// 3. A node is marked if a child was cut.
// 4. If a node is marked and another child is cut, the node itself is cut.
// 5. Once a marked node is itself cut, it loses its marking.
// 6. A tree can only grow through merging of two previous trees.
// 7. At the time of the merge, the trees had the same degree.
// 8. During a merge, one tree becomes the right-most child of the root of 
//    the other tree.
// 9. A child i (from left to right) of the root has at least i-2 children.
//    a. The child can only have been placed under the tree during merging (6).
//    b. During merging, the child had the same degree as the root (7).
//    c. The degree of the root during merging was at least i-1.
//       i. Nodes to the left of child i may have been removed since then
//      through cuts (2, 4).
//       ii. However additions to the left of child i are not possible;
//      only additions to the right of child i are possible (8).
//    d. Thus, during merging the child had a degree of at least i-1 (7).
//    e. Since merging, the child could have lost at most one child;
//       if it lost more, it too would be cut (4).
// 10. Each tree where the root has degree d has size at least O(phi^d).
//     The degree is thus O(lgN), where N is the size of tree.
//     a. We can construct the smallest tree possible for each degree, d,
//     via induction.
//        i. d = 0 is a single node.
//        ii. d = 1 is a node with a child.
//        iii. For a tree of degree d > 1, we are adding a child to the
//           previous tree. The new tree must have degree >=d-2, because
//           when it is added, it will be child d (9). The smallest tree
//           will have degree d-2. The smallest tree of degree d-2 is the
//           tree we previously constructed. Thus, tree of degree d > 1
//           is composed from the prior two trees. 
//     d. The sizes of these trees follows the Fibonnaci sequence.
//     e. The ratio of two adjacent Fibonnaci numbers is at least 1.5 and
//        approaches phi. 
//     f. Thus, a tree (the smallest such tree) of degree d has size
//        >=1.5^d, or ~phi^d. The degree of any tree of size n is thus O(lgn).
// 11. The maximum degree, D, of a heap_t of size N is the degree of a tree
//     that is at least as big as the Fibonnaci tree that contains as
//     many of the nodes as possible. If the tree is O(N), the maximum
//     degree is thus O(lgN) (10).
// 12. The minimum node is always one of the roots of the trees.
//
// AMORTIZED ANALYSIS
//
// For amortized analysis of decrease-key and delete-min, we will use
// the potential method. This requires a potential function such that 
// phi(H_empty) = 0 and the value is always non-negative. Intuitively,
// the function represents the amount of disorder from the ideal state.
// If an operation minimizes chaos, increasing order, we expect the change
// in potential to decrease.
//
// We use the potential function:
//
//    phi(H) = T(H) + 2 * M(H),
//
// where H is the heap, T is the number of trees in H, and M is the number
// of marked nodes in H. This potential function captures the notion that
// having more marks is worse than having more trees, which is useful for
// decrease-key analysis. Additionally, with this function, delete-min
// decreases chaos, and insert and merge increase chaos.
//
// To analyze the amortized cost of an operation, o, we do:
//
//    T_amortized(o) = T_actual(o) + C * (phi(H_before) - phi(H_after)),
//
// where H_before is the heap_t before o, and H_after is the heap_t after o.
//
// FUNCTION DESCRIPTION AND ANALYSIS
//        
// Peek-min: O(1)
// 1. Return the pointer to the minimum. O(1).
//
// Insert(node): O(1)
// 1. Add the node as the root of a degree-0 tree to the heap's forest.
//    This operation is O(1) if the forest is implemented as a doubly-linked
//    lists.
//
// Meld(heap1, heap2): O(1)
// 1. Combine the forests of the input heaps into the forest of the new heap.
//    This operation is O(1) if the forests are implemented as a doubly-linked
//    lists.
// 2. Determine which heap_t has the minimum and set that as the new min. O(1).
//
// Decrease-key(node): O(1) (amortized)
// 1. Change the node's key. O(1).
// 2. If the node has key < min, update the min. O(1).
// 3. If the node's new key is >= the parent's key, stop. O(1).
// 4. Else, cut the node and move to the forest. O(1).
// 5. If the current node does not have a parent, stop. O(1)
// 6. If the parent of the current node is not marked, mark the parent, stop.
//    O(1).
// 7. Else, make the parent the current node, then cut it and move it to the
//    forest. O(1). 
// 8. Return to step 5.
// Amoritized Cost Analysis:
// 1. The actual cost is based on the number of trees created, k.
//    For each tree new tree, a cut was performed, and a node may or
//    may not have been marked. Thus, the final cost is O(k).
// 2. The change in the number of marked nodes before and after the
//    decrease-key operation is between -(k-2) and -k. Except
//    the original node, each node cut was previously marked and now is not.
//    The last node cut may or may not have a parent; if there was a parent,
//    and the parent was not cut, then the parent must have been marked.
//    If the original node was marked, after cutting it, we unmarked it.
//    Thus, the change in the number of marked nodes is -k if the original
//    node was originally marked and later cut and unmarked and there was no
//    final node to mark. The change is -(k-2) if the original node was
//    originally unmarked, and there was a final node that was marked.
// 3. At worst, the change in potential is:
//      phi(H_after) - phi(H_before)
//      = (T(H_after) - T(H_before)) + 2 * (M(H_after) - T(H_before)),
//      = (k) + 2 * (-(k-2))
//      = -k + 4
// 4. Thus, T_amortized_o = O(k) + C * (-k + 4). If C is sufficiently large,
//    we can cancel the O(k) actual cost, leaving a constant time cost, O(1).
//
// Pop-min: O(lgN) (amortized)
// 1. The minimum is always the root of a tree. Cut each of its children
//    and move them to the forest. This is O(D).
// 2. Remove the singleton minimum node. O(1).
// 3. Combine the trees until there is only one tree of each degree.
//    a. Make an array of size D+1, A. O(D).
//    b. Iterate through each tree, t. O(T), where T is the number
//       of trees before combining.
//    c. See if there is a tree at A[d], where d is the degree of t. O(1).
//    d. If there is not a tree in A[d], put t there and move to next tree
//       in forest. O(1).
//    e. Else, the trees have the same degree. Merge the trees and set the
//       new tree as tree t. O(1).
//    f. Repeat from c.
// 4. Find the new minimum among the trees. O(D), since there are now d trees.
// Amortized Cost Analysis:
// 1. The actual cost is O(D + T).
// 2. The change in the number of trees is T - D.
// 3. The change in the number of marked nodes is at best -D, at worst 0,
//    depending on the number of the children of the minimum that were
//    marked before they were cut.
// 4. At worst, the change in potential is:
//      phi(H_after) - phi(H_before)
//      = (T(H_after) - T(H_before)) + 2 * (M(H_after) - T(H_before)),
//      = (T - D) + 2 * (0)
//      = T - D
// 5. Thus, T_amortized_o = O(D + T) + C * (T - D).
//    If C is sufficiently large, we can remove O(T), leaving O(D), which
//    is O(lgN).

struct _impl_heap_node_t {
  item_t *I;

  bool mark;
  struct _impl_heap_node_t *parent;
  // link_t of current node in parent's children linked list
  link_t *parent_children_link;
  linked_list_t *children;
  size_t degree;
};
typedef struct _impl_heap_node_t heap_node_t;

heap_node_t *_heap_node_create(addr_t k, addr_t v) {
  heap_node_t *N = (heap_node_t *) memory_malloc(sizeof(heap_node_t));

  item_t *I = item_create(k, v);
  N->I = I;

  N->mark = false;
  N->parent = NULL;
  N->parent_children_link = NULL;
  N->children = linked_list_create();
  N->degree = 0;

  return N;
}

// Destroy does not own freeing the parent_children_link.
// That is taken care of by delete-min.
void _heap_node_destroy(heap_node_t *N) {
  item_destroy(N->I);
  linked_list_destroy(N->children);
  memory_free(N);
}

size_t TEMP_MAX_STR_SIZE = 50;
size_t HEAP_MAX_STR_SIZE = 1000;

void _print_indent(str_t res, size_t indent_level) {
  for (int i = 0; i < indent_level; i++) {
    strncat(res, "\t", 1);
  }
}

void _print_heap_node(
  str_t res,
  str_t temp,
  size_t indent_level,
  heap_node_t *N,
  str_t (*key_string)(addr_t k),
  str_t (*value_string)(addr_t v)
) {
  size_t temp_len;

  _print_indent(res, indent_level);
  temp_len = 8 + PTR_MAX_STR_SIZE + 1;
  snprintf(temp, temp_len, "heap_node: %p\n", N);
  strncat(res, temp, temp_len);

  item_t *I = N->I;
  assert(I);
  addr_t k = item_get_key(I);
  addr_t v = item_get_value(I);
  str_t k_str = key_string(k);
  str_t v_str = value_string(v);
  size_t k_len = strlen(k_str);
  size_t v_len = strlen(v_str);
  _print_indent(res, indent_level);
  strncat(res, k_str, k_len);
  strncat(res, ":", 1);
  strncat(res, v_str, v_len);
  strncat(res, "\n", 1);

  _print_indent(res, indent_level);
  temp_len = 6 + 1 + 1;
  snprintf(temp, temp_len, "mark: %d\n", N->mark);
  strncat(res, temp, temp_len);

  _print_indent(res, indent_level);
  temp_len = 8 + LU_MAX_STR_SIZE + 1;
  snprintf(temp, temp_len, "degree: %lu\n", N->degree);
  strncat(res, temp, temp_len);

  _print_indent(res, indent_level);
  temp_len = 8 + PTR_MAX_STR_SIZE + 1;
  snprintf(temp, temp_len, "parent: %p\n", N->parent);
  strncat(res, temp, temp_len);

  _print_indent(res, indent_level);
  temp_len = 22 + PTR_MAX_STR_SIZE + 1;
  snprintf(temp, temp_len, "parent_children_link: %p\n", N->parent_children_link);
  strncat(res, temp, temp_len);

  strncat(res, "\n", 1);

  memory_free(k_str);
  memory_free(v_str);
}

struct _impl_heap_t {
  int (*compare)(addr_t e1, addr_t e2);
  bool (*value_eq) (addr_t v1, addr_t v2);
  size_t (*value_hash) (addr_t v);

  size_t len;
  heap_node_t *min;
  linked_list_t *forest;
};

heap_t *heap_create(
  int (*compare)(addr_t e1, addr_t e2),
  bool (*value_eq) (addr_t v1, addr_t v2),
  size_t (*value_hash) (addr_t v)
) {
  heap_t *H = (heap_t *) memory_malloc(sizeof(heap_t));

  H->compare = compare;
  H->value_eq = value_eq;
  H->value_hash = value_hash;

  H->len = 0;
  H->forest = linked_list_create();
  H->min = NULL;

  return H;
}

void heap_destroy(heap_t *H) {
  assert(H);

  linked_list_destroy(H->forest);
  memory_free(H);
}

size_t heap_len(heap_t *H) {
  assert(H);

  return H->len;
}

void _heap_iterator_create_helper(list_t *L, dict_t *D, linked_list_t *ring) {
  link_t *curr = ring->join;
  if (curr == NULL) {
    return;
  }

  heap_node_t *N;
  item_t *iteration;
  item_t *I;
  item_t *It;
  addr_t k;
  addr_t v;

  do {
    N = (heap_node_t *) curr->value;
    k = item_get_key(N->I);
    v = item_get_value(N->I);

    It = dict_get(D, v);
    assert(It == NULL);

    I = item_create(k, N);
    dict_set(D, v, I);
    iteration = item_create(v, I);
    list_push(L, iteration);

    _heap_iterator_create_helper(L, D, N->children);

    curr = curr->next;
  } while (curr != ring->join);
}

list_t *heap_iterator_create(heap_t *H) {
  assert(H);

  list_t *L = list_create(0);
  dict_t *D = dict_create(H->value_eq, H->value_hash);
  _heap_iterator_create_helper(L, D, H->forest);
  dict_destroy(D);
  return L;
}

void heap_iterator_destroy(list_t *iterator) {
  assert(iterator);

  item_t *iteration;
  item_t *I;

  for (int i = 0; i < list_len(iterator); i++) {
    iteration = list_get(iterator, i);
    I = item_get_value(iteration);
    memory_free(I);
    memory_free(iteration);
  }
  list_destroy(iterator);
}

item_t *heap_peek_min(heap_t *H) {
  assert(H);

  heap_node_t *N = H->min;
  if (N == NULL) {
    return NULL;
  }
  return N->I;
}

void _heap_update_min(heap_t *H, addr_t k, heap_node_t *N) {
  if (H->min == NULL) {
    H->min = N;
  } else if (H->compare(item_get_key(H->min->I), k) < 0) {
    H->min = N; 
  }
}

void heap_insert(heap_t *H, addr_t k, addr_t v) {
  assert(H);

  H->len += 1;

  heap_node_t *N = _heap_node_create(k, v);
  link_t *parent_children_link = linked_list_push(H->forest, N);
  N->parent_children_link = parent_children_link;

  _heap_update_min(H, k, N);
}

// Return whether parent needs to be cut.
bool _heap_cut(heap_t *H, heap_node_t *N) {
  assert(N->parent);

  bool cut_parent = false;
  if (!N->parent->mark) {
    N->parent->mark = true;
  } else {
    cut_parent = true;
  }

  linked_list_remove(N->parent->children, N->parent_children_link);
  N->parent_children_link = NULL;
  N->parent->degree -= 1;
  N->parent = NULL;
  N->mark = false;

  link_t *parent_children_link = linked_list_push(H->forest, N);
  N->parent_children_link = parent_children_link;

  return cut_parent;
}

heap_node_t *_heap_merge(heap_t *H, heap_node_t *N1, heap_node_t *N2) {
  assert(N1->parent == NULL);
  assert(N2->parent == NULL);
  assert(N1->degree == N2->degree);

  heap_node_t *parent;
  heap_node_t *child;

  if (H->compare(item_get_key(N2->I), item_get_key(N1->I)) <= 0) {
    parent = N1;
    child = N2;
  } else {
    parent = N2;
    child = N1;
  }

  link_t *parent_children_link = linked_list_push(parent->children, child);
  parent->degree += 1;
  child->parent = parent;
  child->parent_children_link = parent_children_link;
  return parent;
}

void _heap_merge_up(heap_t *H, list_t *min_trees, heap_node_t *N) {
  size_t d;
  heap_node_t *merged;
  heap_node_t *curr;
  heap_node_t *prev;

  curr = N;
  d = curr->degree;
  prev = list_get(min_trees, d);

  while (prev != NULL) {
    // remove prev from array
    list_set(min_trees, d, NULL);

    merged = _heap_merge(H, curr, prev);
    curr = merged;
    d = curr->degree;
    prev = list_get(min_trees, d);
  };
  list_set(min_trees, d, curr);
}

void heap_decrease_key(heap_t *H, heap_node_t *N, addr_t k) {
  assert(H);
  assert(N);
  // ensure key does not increase
  assert(H->compare(item_get_key(N->I), k) <= 0);

  item_set_key(N->I, k);
  _heap_update_min(H, k, N);

  if (N->parent == NULL) {
    return;
  }
  if (H->compare(item_get_key(N->parent->I), k) >= 0) {
    return;
  }
  bool cut_parent = _heap_cut(H, N);

  heap_node_t *curr = N->parent;
  while (curr != NULL && cut_parent) {
    cut_parent = _heap_cut(H, curr);
    curr = curr->parent;
  }
}

void heap_delete_min(heap_t *H) {
  assert(H);

  if (H->len == 0) {
    return;
  }
  assert(H->min != NULL);

  link_t *L;
  heap_node_t *N;

  while (!linked_list_empty(H->min->children)) {
    L = H->min->children->join;
    N = (heap_node_t *) L->value;
    assert(N);

    _heap_cut(H, N);
  }

  linked_list_remove(H->forest, H->min->parent_children_link);
  _heap_node_destroy(H->min);
  H->min = NULL;
  H->len -= 1;
  if (H->len == 0) {
    return;
  }

  // max-degree <= log_1.5(x) = log_2(x) / log_2(1.5) < 2 * log_2(x)
  size_t upper_bound = 2 * lu_log_2(H->len) + 1;
  list_t *min_trees = list_create(upper_bound + 1); // Account for degree = 0.

  L = H->forest->join;
  do {
    N = (heap_node_t *) L->value;
    _heap_merge_up(H, min_trees, N);
    L = L->next;
  } while (L != H->forest->join);

  link_t *parent_children_link;

  linked_list_t *forest = linked_list_create();
  for (int i = 0; i < list_len(min_trees); i++) {
    N = list_get(min_trees, i);
    if (N == NULL) {
      continue;
    }
    parent_children_link = linked_list_push(forest, N);
    N->parent_children_link = parent_children_link;
  }
  list_destroy(min_trees);
  while (!linked_list_empty(H->forest)) {
    linked_list_remove(H->forest, H->forest->join);
  }
  linked_list_destroy(H->forest);
  H->forest = forest;

  L = H->forest->join;
  do {
    N = (heap_node_t *) L->value; 
    _heap_update_min(H, item_get_key(N->I), N);
    L = L->next;
  } while (L != H->forest->join);
}

heap_t *heap_meld(
  heap_t *H1,
  heap_t *H2,
  int (*compare)(addr_t k1, addr_t k2),
  bool (*value_eq) (addr_t v1, addr_t v2),
  size_t (*value_hash) (addr_t v)
) {
  assert(H1);
  assert(H2);

  heap_t *H = heap_create(compare, value_eq, value_hash);

  H->len = H1->len + H2->len;
  H->min = NULL;
  if (H1->min == NULL) {
    H->min = H2->min;
  } else if (H2->min == NULL) {
    H->min = H1->min;
  } else {
    if (compare(item_get_key(H2->min->I), item_get_key(H1->min->I)) <= 0) {
      H->min = H1->min;
    } else {
      H->min = H2->min;
    }
  }

  linked_list_t *combined_forest = linked_list_combine(H1->forest, H2->forest);
  linked_list_destroy(H->forest);
  H->forest = combined_forest;

  return H;
}

// DFS-traversal of nodes.
void _heap_string_helper(
  str_t res,
  str_t temp,
  size_t indent_level,
  linked_list_t *ring,
  str_t (*key_string)(addr_t k),
  str_t (*value_string)(addr_t v)
) {
  link_t *curr = ring->join;
  if (curr == NULL) {
    return;
  }

  heap_node_t *N;

  do {
    N = (heap_node_t *) curr->value;
    _print_heap_node(res, temp, indent_level, N, key_string, value_string);
    _heap_string_helper(res, temp, indent_level+1, N->children, key_string, value_string);

    curr = curr->next;
  } while (curr != ring->join);
}

str_t heap_string(heap_t *H, str_t (*key_string)(addr_t k), str_t (*value_string)(addr_t v)) {
  assert(H);

  str_t res = (str_t) memory_calloc(HEAP_MAX_STR_SIZE, sizeof(char));
  str_t temp = memory_calloc(TEMP_MAX_STR_SIZE, sizeof(char));
  strncat(res, "<\n", 2);
  _heap_string_helper(res, temp, 1, H->forest, key_string, value_string);
  strncat(res, ">\n", 2);
  memory_free(temp);
  return res;
}
