#ifndef NAIVE_TSEARCH_PREFIX
# error "NAIVE_TSEARCH_PREFIX macro not defined"
#endif

#ifndef NAIVE_TSEARCH_API
# error "NAIVE_TSEARCH_API macro not defined"
#endif

#define NAIVE_TSEARCH_CONCAT(PREF, NAME) PREF ## NAME
#define NAIVE_TSEARCH_CONCAT_DEREF(PREFIX, NAME) NAIVE_TSEARCH_CONCAT(PREFIX, NAME)
#define NAIVE_TSEARCH_PREPEND(NAME) NAIVE_TSEARCH_CONCAT_DEREF(NAIVE_TSEARCH_PREFIX, NAME)

#define NAIVE_TSEARCH_VISIT        NAIVE_TSEARCH_PREPEND(VISIT)
#define NAIVE_TSEARCH_TSEARCH      NAIVE_TSEARCH_PREPEND(tsearch)
#define NAIVE_TSEARCH_TFIND        NAIVE_TSEARCH_PREPEND(tfind)
#define NAIVE_TSEARCH_TDELETE      NAIVE_TSEARCH_PREPEND(tdelete)
#define NAIVE_TSEARCH_TWALK        NAIVE_TSEARCH_PREPEND(twalk)
#define NAIVE_TSEARCH_TDESTROY     NAIVE_TSEARCH_PREPEND(tdestroy)

typedef enum { preorder, postorder, endorder, leaf } NAIVE_TSEARCH_VISIT;

NAIVE_TSEARCH_API
void *NAIVE_TSEARCH_TSEARCH(const void *key, void **rootp,
    int (*compar)(const void *, const void *));

NAIVE_TSEARCH_API
void *NAIVE_TSEARCH_TFIND(const void *key, void *const *rootp,
    int (*compar)(const void *, const void *));

NAIVE_TSEARCH_API
void *NAIVE_TSEARCH_TDELETE(const void *key, void **rootp,
    int (*compar)(const void *, const void *));

NAIVE_TSEARCH_API
void NAIVE_TSEARCH_TWALK(const void *root,
   void (*action)(const void *nodep, NAIVE_TSEARCH_VISIT which, int depth));

NAIVE_TSEARCH_API
void NAIVE_TSEARCH_TDESTROY(void *root, void (*free_node)(void *nodep));
