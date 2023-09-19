/**
 * MIT License
 *
 * Copyright (c) 2023 Mathieu Rabine
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// rbtree.
#include <rbtree.h>

// C.
#include <stdlib.h>
#include <stdio.h>

// =========================================================================
//   CLASS     :
//   METHOD    : is_red
// =========================================================================
int is_red (struct rbnode* node)
{
    return node ? node->red : 0;
}

// =========================================================================
//   CLASS     :
//   METHOD    : make_node
// =========================================================================
struct rbnode* make_node (void* data)
{
    struct rbnode* node = malloc (sizeof (struct rbnode));

    if (node != NULL)
    {
        node->link[0] = node->link[1] = node->parent = NULL;
        node->data = data;
        node->red = 1;
    }

    return node;
}

// =========================================================================
//   CLASS     :
//   METHOD    : single_rotate
// =========================================================================
struct rbnode* single_rotate (struct rbnode* node, int dir)
{
    struct rbnode* save = node->link[!dir];

    if (save != NULL)
    {
        node->link[!dir] = save->link[dir];
        if (node->link[!dir] != NULL)
            node->link[!dir]->parent = node;
        save->parent = node->parent;
        save->link[dir] = node;
        node->parent = save;
        node->red = 1;
        save->red = 0;
    }

    return save;
}

// =========================================================================
//   CLASS     :
//   METHOD    : double_rotate
// =========================================================================
struct rbnode* double_rotate (struct rbnode* node, int dir)
{
    node->link[!dir] = single_rotate (node->link[!dir], !dir);
    return single_rotate (node, dir);
}

// =========================================================================
//   CLASS     :
//   METHOD    : rb_new
// =========================================================================
struct rbtree* rb_new (rb_compare* compare, rb_destroy* destroy)
{
    struct rbtree* tree = malloc (sizeof (struct rbtree));

    if (tree != NULL)
    {
        tree->comp  = compare;
        tree->del   = destroy;
        tree->root  = NULL;
        tree->count = 0;
    }

    return tree;
}

// =========================================================================
//   CLASS     :
//   METHOD    : rb_delete
// =========================================================================
void rb_delete (struct rbtree* tree)
{
    if (tree != NULL)
    {
        struct rbnode* node = tree->root;
        struct rbnode* save = NULL;

        while (node != NULL)
        {
            if (node->link[0] == NULL)
            {
                save = node->link[1];
                if (tree->del != NULL && node->data != NULL)
                    tree->del (node->data);
                free (node);
                node = NULL;
            }
            else
            {
                save = node->link[0];
                node->link[0] = save->link[1];
                save->link[1] = node;
            }

            node = save;
        }

        free (tree);
    }
}

// =========================================================================
//   CLASS     :
//   METHOD    : rb_insert
// =========================================================================
void* rb_insert (void* data, struct rbtree* tree)
{
    void* inserted = NULL;

    if (tree->root == NULL)
    {
        tree->root = make_node (data);
        if (tree->root == NULL)
        {
            return NULL;
        }

        inserted = tree->root->data;
        ++tree->count;
    }
    else
    {
        struct rbnode head = { 0 };
        struct rbnode *g, *t;
        struct rbnode *p, *q;
        int dir = 0, last, comp;

        t = &head;
        g = p = NULL;
        q = t->link[1] = tree->root;

        for (;;)
        {
            if (q == NULL)
            {
                p->link[dir] = q = make_node (data);
                if (q == NULL)
                {
                    return NULL;
                }
                q->parent = p;
                inserted = q->data;
                ++tree->count;
            }
            else if (is_red (q->link[0]) && is_red (q->link[1]))
            {
                q->red = 1;
                q->link[0]->red = q->link[1]->red = 0;
            }

            if (is_red (q) && is_red (p))
            {
                int dir2 = t->link[1] == g;

                if (q == p->link[last])
                {
                    t->link[dir2] = single_rotate (g, !last);
                }
                else
                {
                    t->link[dir2] = double_rotate (g, !last);
                }
            }

            if (inserted != NULL || (comp = tree->comp (q->data, data)) == 0)
            {
                break;
            }

            last = dir;
            dir = comp < 0;

            if (g != NULL)
            {
                t = g;
            }

            g = p, p = q;
            q = q->link[dir];
        }

        tree->root = head.link[1];
    }

    tree->root->red = 0;

    return inserted;
}

// =========================================================================
//   CLASS     :
//   METHOD    : rb_find
// =========================================================================
void* rb_find (const void* data, struct rbtree* tree)
{
    if (tree != NULL)
    {
        struct rbnode* node = tree->root;
        int comp;

        while (node != NULL)
        {
            if ((comp = tree->comp (node->data, data)) == 0)
                return node->data;
            node = node->link[comp < 0];
        }
    }

    return NULL;
}

// =========================================================================
//   CLASS     :
//   METHOD    : rb_remove
// =========================================================================
void rb_remove (const void* data, struct rbtree* tree)
{
    if (tree->root != NULL)
    {
        struct rbnode head = { 0 };
        struct rbnode *q, *p, *g;
        struct rbnode *f = NULL;
        int dir = 1;

        q = &head;
        g = p = NULL;
        q->link[1] = tree->root;

        while (q->link[dir] != NULL)
        {
            int last = dir;

            g = p, p = q;
            q = q->link[dir];

            int comp = tree->comp (q->data, data);
            dir = comp < 0;

            if (comp == 0)
            {
                f = q;
            }

            if (!is_red (q) && !is_red (q->link[dir]))
            {
                if (is_red (q->link[!dir]))
                {
                    p = p->link[last] = single_rotate (q, dir);
                }
                else if (!is_red (q->link[!dir]))
                {
                    struct rbnode* s = p->link[!last];

                    if (s != NULL)
                    {
                        if (!is_red (s->link[!last]) && !is_red (s->link[last]))
                        {
                            p->red = 0;
                            s->red = 1;
                            q->red = 1;
                        }
                        else
                        {
                            int dir2 = g->link[1] == p;

                            if (is_red (s->link[last]))
                            {
                                g->link[dir2] = double_rotate (p, last);
                            }
                            else if (is_red (s->link[!last]))
                            {
                                g->link[dir2] = single_rotate (p, last);
                            }

                            q->red = g->link[dir2]->red = 1;
                            g->link[dir2]->link[0]->red = 0;
                            g->link[dir2]->link[1]->red = 0;
                        }
                    }
                }
            }
        }

        if (f != NULL)
        {
            if (tree->del != NULL && f->data != NULL)
                tree->del (f->data);
            f->data = q->data;
            p->link[p->link[1] == q] = q->link[q->link[0] == NULL];
            --tree->count;
            free (q);
        }

        tree->root = head.link[1];

        if (tree->root != NULL)
        {
            tree->root->red = 0;
        }
    }
}

// =========================================================================
//   CLASS     :
//   METHOD    : rb_empty
// =========================================================================
int rb_empty (struct rbtree* tree)
{
    if (tree != NULL)
    {
        return rb_size (tree) == 0;
    }

    return 1;
}

// =========================================================================
//   CLASS     :
//   METHOD    : rb_size
// =========================================================================
size_t rb_size (struct rbtree* tree)
{
    if (tree != NULL)
    {
        return tree->count;
    }

    return 0;
}
