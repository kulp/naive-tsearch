// From Algorithms T and D, Knuth, TAoCP Vol. 3 (6.2.2)

#include <stdlib.h>

struct naive_tsearch_tree {
    // datum must be the first field in struct naive_tsearch_tree
    const void *datum;
    struct naive_tsearch_tree *left, *right;
};

static struct naive_tsearch_tree *naive_tsearch_null_tree_p = NULL;

typedef int  naive_tsearch_cmp(const void *key1, const void *key2);
typedef void naive_tsearch_act(const void *node, NAIVE_TSEARCH_VISIT order, int level);
typedef void naive_tsearch_freer(void *node);

// guaranteed to return a non-NULL pointer (might be a pointer to NULL)
static struct naive_tsearch_tree **naive_tsearch_traverse(const void *key, struct naive_tsearch_tree **rootp, naive_tsearch_cmp *compar,
                              int create, struct naive_tsearch_tree **parent)
{
    if (*rootp == NULL) {
        if (create) {
            struct naive_tsearch_tree *q = *rootp = (struct naive_tsearch_tree *) malloc(sizeof *q);
            q->left = q->right = NULL;
            q->datum = key;
            return rootp;
        } else {
            return &naive_tsearch_null_tree_p;
        }
    }

    struct naive_tsearch_tree *p = *rootp;
    int result = compar(key, p->datum);
    if (parent) *parent = p;
    // we could easily implement this iteratively as well but let's do it
    // recursively and depend on a smart compiler to use tail recursion
    if (result == 0) {
        return rootp;
    } else if (result < 0) {
        return naive_tsearch_traverse(key, &p->left , compar, create, parent);
    } else {
        return naive_tsearch_traverse(key, &p->right, compar, create, parent);
    }
}

NAIVE_TSEARCH_API
void *NAIVE_TSEARCH_TFIND(const void *key, void *const *rootp, naive_tsearch_cmp *compar)
{
    return *naive_tsearch_traverse(key, (struct naive_tsearch_tree**)rootp, compar, 0, NULL);
}

NAIVE_TSEARCH_API
void *NAIVE_TSEARCH_TSEARCH(const void *key, void **rootp, naive_tsearch_cmp *compar)
{
    return *naive_tsearch_traverse(key, (struct naive_tsearch_tree**)rootp, compar, 1, NULL);
}

NAIVE_TSEARCH_API
void *NAIVE_TSEARCH_TDELETE(const void *__restrict key, void **__restrict rootp, naive_tsearch_cmp *compar)
{
    struct naive_tsearch_tree *parent = NULL;
    struct naive_tsearch_tree **q = naive_tsearch_traverse(key, (struct naive_tsearch_tree**)rootp, compar, 0, &parent);
    if (!*q || !(*q)->datum)
        return NULL;

    struct naive_tsearch_tree *t = *q;
    do {
        if (!t->right) {
            *q = t->left;
            break;
        }

        struct naive_tsearch_tree *r = t->right;
        if (!r->left) {
            r->left = t->left;
            *q = r;
            break;
        }

        struct naive_tsearch_tree *s = r->left;
        while (s->left) {
            r = s;
            s = r->left;
        }
        s->left = t->left;
        r->left = s->right;
        s->right = t->right;
        *q = s;
    } while (0);

    free(t);

    return parent;
}

// naive_tsearch_walk(), unlike naive_tsearch_traverse(), cannot tail-recurse, and so we might want an
// iterative implementation for large trees
static void naive_tsearch_walk(const void *root, naive_tsearch_act *action, int level)
{
    const struct naive_tsearch_tree *p = (const struct naive_tsearch_tree *)root;
    if (!p) return;

    if (!p->left && !p->right) {
        action(p, leaf, level);
    } else {
        action(p, preorder , level);
        naive_tsearch_walk(p->left , action, level + 1);
        action(p, postorder, level);
        naive_tsearch_walk(p->right, action, level + 1);
        action(p, endorder , level);
    }
}

NAIVE_TSEARCH_API
void NAIVE_TSEARCH_TWALK(const void *root, naive_tsearch_act *action)
{
    naive_tsearch_walk(root, action, 0);
}

NAIVE_TSEARCH_API
void NAIVE_TSEARCH_TDESTROY(void *root, naive_tsearch_freer *free_node)
{
    struct naive_tsearch_tree *p = (struct naive_tsearch_tree *)root;
    if (!p)
        return;

    NAIVE_TSEARCH_TDESTROY(p->left , free_node);
    NAIVE_TSEARCH_TDESTROY(p->right, free_node);
    free_node((void*)p->datum);
    free(p);
}
