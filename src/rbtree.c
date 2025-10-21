#include "rbtree.h"

#include <stdlib.h>

static void rbtree_fixup(rbtree *t, node_t *z);
static void right_rotate(rbtree *t, node_t *z);
static void left_rotate(rbtree *t, node_t *z);
static void reclaim_nodes(rbtree *t, node_t *node);
static void transplant(rbtree *t, node_t *u, node_t *v);
static void delete_fixup(rbtree *t, node_t *x);
static node_t *tree_minimum(rbtree *t, node_t *node);

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (!p)
    return NULL;

  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  if (!nil)
  {
    free(p);
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
  if (!t)
    return;

  reclaim_nodes(t, t->root);
  free(t->nil);
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
  node_t *curr = t->root;

  while (curr != t->nil)
  {
    if (key == curr->key)
      return curr;
    else if (key > curr->key)
      curr = curr->right;
    else
      curr = curr->left;
  }

  return NULL;
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
  node_t *y = p;
  node_t *x;
  color_t y_original_color = y->color;

  if (p->left == t->nil)
  {
    x = p->right;
    transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    x = p->left;
    transplant(t, p, p->left);
  }
  else
  {
    y = tree_minimum(t, p->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent != p)
    {
      transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    else
    {
      x->parent = y;
    }

    transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_original_color == RBTREE_BLACK)
    delete_fixup(t, x);

  free(p);
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

void reclaim_nodes(rbtree *t, node_t *node)
{
  if (node == t->nil)
    return;

  reclaim_nodes(t, node->left);
  reclaim_nodes(t, node->right);

  free(node);
}

void transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

void delete_fixup(rbtree *t, node_t *x)
{
  node_t *w;

  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      w = x->parent->right;

      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      w = x->parent->left;

      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}

node_t *tree_minimum(rbtree *t, node_t *node);
{
  while (node->left != t->nil)
    node = node->left;
  return node;
}