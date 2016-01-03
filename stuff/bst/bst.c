/*
 * Copyright (c) 2015, Carsten Kunze
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

#define NODE_FOUND   0
#define INSERT_LEFT  1
#define INSERT_RIGHT 2
#define TREE_EMPTY   3

static int srch_node(struct bst *, union bst_val, struct bst_node **);
static struct bst_node *alloc_node(void);
static void *malloc_exit(size_t size);

/* Returns:
 *   0           No error
 *   BST_EEXIST  Key already exists
 *   BST_EINTL   Internal error
 */

int
bst_add(struct bst *bst, union bst_val key, union bst_val data) {
	struct bst_node *n, *c, *gc, *t;
	switch (srch_node(bst, key, &n)) {
	case NODE_FOUND:
		fprintf(stderr, "bst_add: Key does already exist\n");
		return BST_EEXIST;
	case INSERT_LEFT:
		c = alloc_node();
		n->left = c;
		break;
	case INSERT_RIGHT:
		c = alloc_node();
		n->right = c;
		break;
	case TREE_EMPTY:
		c = alloc_node();
		bst->root = c;
		break;
	default:
		fprintf(stderr, "BST: Unexpected internal srch_node error\n");
		return BST_EINTL;
	}
	c->parent = n;
	c->key = key;
	c->data = data;
	while (n) {
		if (n->left == c)
			n->bf++;
		else
			n->bf--;
		if (!n->bf)
			break;
		if (n->bf == -2) {
			if (c->bf == -1) {
				if (!(t = c->parent = n->parent))
					bst->root = c;
				else if (t->left == n)
					t->left = c;
				else
					t->right = c;
				n->parent = c;
				if ((t = n->right = c->left))
					t->parent = n;
				c->left = n;
				n->bf = c->bf = 0;
			} else if (c->bf == 1) {
				if ((t = c->left = gc->right))
					t->parent = c;
				gc->right = c;
				c->parent = gc;
				if (!(t = gc->parent = n->parent))
					bst->root = gc;
				else if (t->left == n)
					t->left = gc;
				else
					t->right = gc;
				n->parent = gc;
				if ((t = n->right = gc->left))
					t->parent = n;
				gc->left = n;
				if (!gc->bf)
					n->bf = c->bf = 0;
				else if (gc->bf > 0) {
					n->bf = 0;
					c->bf = -1;
				} else {
					n->bf = 1;
					c->bf = 0;
				}
				gc->bf = 0;
			}
			break;
		}
		if (n->bf == 2) {
			if (c->bf == 1) {
				if (!(t = c->parent = n->parent))
					bst->root = c;
				else if (t->left == n)
					t->left = c;
				else
					t->right = c;
				n->parent = c;
				if ((t = n->left = c->right))
					t->parent = n;
				c->right = n;
				n->bf = c->bf = 0;
			} else if (c->bf == -1) {
				if ((t = c->right = gc->left))
					t->parent = c;
				gc->left = c;
				c->parent = gc;
				if (!(t = gc->parent = n->parent))
					bst->root = gc;
				else if (t->left == n)
					t->left = gc;
				else
					t->right = gc;
				n->parent = gc;
				if ((t = n->left = gc->right))
					t->parent = n;
				gc->right = n;
				if (!gc->bf)
					n->bf = c->bf = 0;
				else if (gc->bf > 0) {
					c->bf = 0;
					n->bf = -1;
				} else {
					c->bf = 1;
					n->bf = 0;
				}
				gc->bf = 0;
			}
			break;
		}
		gc = c;
		c = n;
		n = n->parent;
	}
	return 0;
}

/* Unbalanced node delete.  Will be reimplemented AVL conform eventually.
 *
 * Returns:
 *   0           No error
 *   BST_ENOENT  Key not found
 */
int
bst_del(struct bst *bst, union bst_val key) {
	struct bst_node *n;
	if (srch_node(bst, key, &n) != NODE_FOUND) {
		return BST_ENOENT;
	}
	return bst_del_node(bst, n);
}

int
bst_del_node(struct bst *bst, struct bst_node *n) {
	struct bst_node *p, **pp, *t;
	if (!n->parent) {
		p = NULL;
		pp = &bst->root;
	} else {
		p = n->parent;
		if (p->left == n)
			pp = &p->left;
		else
			pp = &p->right;
	}
	if (!n->left) {
		*pp = n->right;
		if (n->right)
			n->right->parent = p;
	} else {
		*pp = n->left;
		n->left->parent = p;
		if (n->right) {
			n->right->parent = n->left;
			if (n->left->right) {
				for (t = n->right; t->left; t = t->left)
					;
				t->left = n->left->right;
				n->left->right->parent = t;
			}
			n->left->right = n->right;
		}
	}
	free(n);
	return 0;
}

int /* 0: found, !0: not found */
bst_srch(struct bst *bst, union bst_val key, struct bst_node **node)
{
	struct bst_node *n;
	int retval;
	if ((retval = srch_node(bst, key, &n)) == NODE_FOUND && node)
		*node = n;
	return retval;
}

/* Returns:
 *   NODE_FOUND    Found in node
 *   INSERT_LEFT   Not found, insert to node->left
 *   INSERT_RIGHT  Not found, insert to node->right
 *   TREE_EMPTY    Tree empty (node = NULL) */

static int
srch_node(struct bst *bst, union bst_val key, struct bst_node **node) {
	struct bst_node *n = bst->root, *c;
	int retval = TREE_EMPTY;
	int d;
	while (n) {
		d = bst->cmp(key, n->key);
		if (d < 0) {
			c = n->left;
		} else if (d > 0) {
			c = n->right;
		} else {
			retval = NODE_FOUND;
			goto end;
		}
		if (!c) {
			retval = d > 0 ? INSERT_RIGHT : INSERT_LEFT;
			goto end;
		}
		n = c;
	}
end:
	*node = n;
	return retval;
}

static struct bst_node *
alloc_node(void) {
	struct bst_node *n = malloc_exit(sizeof(struct bst_node));
	n->left = n->right = NULL;
	n->bf = 0;
	return n;
}

static void *
malloc_exit(size_t size) {
	void *p;
	if (!(p = malloc(size))) {
		fprintf(stderr, "Out of memory\n");
		exit(1);
	}
	return p;
}
