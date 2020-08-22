#include <boost/test/unit_test.hpp>

#include "test_tsearch.hpp"

struct record {
    unsigned key;
};

static int record_comp(const void *o1, const void *o2) {
    return static_cast<const record *>(o2)->key - static_cast<const record *>(o1)->key;
}

BOOST_AUTO_TEST_CASE(tdelete_simple) {
    void *root = NULL;
    // This example was taken from:
    // https://en.wikipedia.org/wiki/Binary_search_tree#/media/File:Binary_search_tree.svg
    tsearch(new record{ 8}, &root, record_comp);

    tsearch(new record{ 3}, &root, record_comp);
    tsearch(new record{10}, &root, record_comp);

    tsearch(new record{ 1}, &root, record_comp);
    tsearch(new record{ 6}, &root, record_comp);
    tsearch(new record{14}, &root, record_comp);

    tsearch(new record{ 4}, &root, record_comp);
    tsearch(new record{ 7}, &root, record_comp);
    tsearch(new record{13}, &root, record_comp);

    // delete entire tree
    int counter = 0;
    while (root) {
        counter++;
        tdelete(*(void**)root, &root, record_comp);
    }

    BOOST_REQUIRE_EQUAL(root, nullptr);
    BOOST_REQUIRE_EQUAL(counter, 9);
}

