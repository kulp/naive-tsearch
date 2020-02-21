#ifndef TSEARCH_H_
#define TSEARCH_H_

#include "tsearch_cfg.h"

#if HAS_INCLUDE_SEARCH_H
#include <search.h>
#endif

#if !HAS_ENUM_VISIT
typedef enum { preorder, postorder, endorder, leaf } VISIT;
#endif

#if !HAS_FUNCTION_TSEARCH
void *tsearch(const void *key, void **rootp,
    int (*compar)(const void *, const void *));
#endif

#if !HAS_FUNCTION_TFIND
void *tfind(const void *key, void *const *rootp,
    int (*compar)(const void *, const void *));
#endif

#if !HAS_FUNCTION_TDELETE
void *tdelete(const void *key, void **rootp,
    int (*compar)(const void *, const void *));
#endif

#if !HAS_FUNCTION_TWALK
void twalk(const void *root,
   void (*action)(const void *nodep, VISIT which, int depth));
#endif

#endif // TSEARCH_H_
