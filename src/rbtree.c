#include "rbtree.h"

#include <stdlib.h>

static void rbtree_fixup(rbtree *t, node_t *z);
static void right_rotate(rbtree *t, node_t *z);
static void left_rotate(rbtree *t, node_t *z);

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (!p)
    return NULL;

  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  if (!nil)
  {
    free(p); // 이미 할당된 메모리 해제
    return NULL;
  }

  nil->color = RBTREE_BLACK;
  nil->left = nil->right = nil->parent = nil;
  p->root = nil;
  p->nil = nil;

  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  node_t *x = t->root;
  node_t *y = t->nil;

  while (x != t->nil)
  {
    y = x;
    if (z->key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  z->parent = y;

  if (y == t->nil)
    t->root = z;
  else if (z->key < y->key)
    y->left = z;
  else
    y->right = z;

  rbtree_fixup(t, z);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}

void rbtree_fixup(rbtree *t, node_t *z)
{
  node_t *y;

  while (z != t->root && z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      y = z->parent->parent->right;

      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          z = z->parent;
          left_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else
    {
      y = z->parent->parent->left;
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

void left_rotate(rbtree *t, node_t *z)
{
  node_t *y = z->right;
  z->right = y->left;
  if (y->left != t->nil)
    y->left->parent = z;

  y->parent = z->parent;

  if (z->parent == t->nil)
    t->root = y;
  else if (z == z->parent->left)
    z->parent->left = y;
  else
    z->parent->right = y;

  y->left = z;
  z->parent = y;
}

void right_rotate(rbtree *t, node_t *y)
{
  node_t *x = y->left;
  y->left = x->right;
  if (x->right != t->nil)
    x->right->parent = y;

  x->parent = y->parent;

  if (y->parent == t->nil)
    t->root = x;
  else if (y == y->parent->right)
    y->parent->right = x;
  else
    y->parent->left = x;

  x->right = y;
  y->parent = x;
}
