#include "tsearch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct record {
    int key;
    char val[16];
};

int comp(const void *_a, const void *_b)
{
    const struct record *a = _a;
    const struct record *b = _b;

    return a->key - b->key;
}

void act(const void *what, VISIT kind, int level)
{
    const struct record * const *record = what;
    static const char *name[] = {
        [preorder ] = "preorder",
        [postorder] = "postorder",
        [endorder ] = "endorder",
        [leaf     ] = "leaf",
    };

    printf("key = %d, kind = %s, level = %d\n", (*record)->key, name[kind], level);
}

static void insert_node(void **rootp)
{
    struct record *r = malloc(sizeof *r);
    r->key = (rand() >> 16) % 100;
    snprintf(r->val, sizeof r->val, "value is '%d'", r->key);
    tsearch(r, rootp, comp);
}

static void noop() { }

int main(void)
{
    void *root = NULL;

    int i;
    for (i = 0; i < 20; i++)
        insert_node(&root);

    // TODO test tfind() explicitly

    twalk(root, act);

    // TODO test tdelete()

#if __gnu_linux__
    tdestroy(root, free);
#endif

    return 0;
}
