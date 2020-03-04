#ifndef TEST_TSEARCH_HPP
#define TEST_TSEARCH_HPP

#if defined(TEST_NAIVE_TSEARCH)
#include "tsearch.h"
#elif defined(TEST_NAIVE_TSEARCH_HDRONLY)
#include "tsearch_hdronly.h"
#elif defined(TEST_SYSTEM_TSEARCH)
#include <search.h>
#else
#error "Don't know what tsearch implementation to test"
#endif

#endif  // TEST_TSEARCH_HPP
