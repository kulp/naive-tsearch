#include <boost/test/unit_test.hpp>

#include "test_tsearch.hpp"

struct record {
    unsigned key;
};

struct walk_item {
    walk_item(const void *w, VISIT k, int l) : what(w), kind(k), level(l) {}
    const void *what;
    VISIT kind;
    int level;
};

static int record_comp(const void *o1, const void *o2) {
    return static_cast<const record *>(o2)->key - static_cast<const record *>(o1)->key;
}

static int record_comp_reverse(const void *o1, const void *o2) {
    return static_cast<const record *>(o1)->key - static_cast<const record *>(o2)->key;
}

BOOST_AUTO_TEST_SUITE(integration_suite)

std::vector<walk_item> v_walk_tree_empty;
static void act_walk_tree_empty(const void *what, VISIT kind, int level) {
    v_walk_tree_empty.emplace_back(what, kind, level);
}

BOOST_AUTO_TEST_CASE(walk_tree_empty) {
    v_walk_tree_empty.clear();
    void *root = NULL;
    twalk(root, act_walk_tree_empty);
    BOOST_CHECK_EQUAL(v_walk_tree_empty.size(), 0);
}

std::vector<walk_item> v_walk_tree_one;
static void act_walk_tree_one(const void *what, VISIT kind, int level) {
    v_walk_tree_one.emplace_back(what, kind, level);
}

BOOST_AUTO_TEST_CASE(walk_tree_one) {
    v_walk_tree_one.clear();
    void *root = NULL;
    record *r = new record{0xcafebabe};
    tsearch(r, &root, record_comp);
    twalk(root, act_walk_tree_one);
    BOOST_REQUIRE_EQUAL(v_walk_tree_one.size(), 1);

    BOOST_REQUIRE_NE(v_walk_tree_one[0].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_one[0].what), r);
    BOOST_CHECK_EQUAL(v_walk_tree_one[0].kind, leaf);
    BOOST_CHECK_EQUAL(v_walk_tree_one[0].level, 0);
}

std::vector<walk_item> v_walk_tree_two;
static void act_walk_tree_two(const void *what, VISIT kind, int level) {
    v_walk_tree_two.emplace_back(what, kind, level);
}

BOOST_AUTO_TEST_CASE(walk_tree_two) {
    v_walk_tree_two.clear();
    void *root = NULL;
    record *r1 = new record{0};
    tsearch(r1, &root, record_comp);
    record *r2 = new record{1};
    tsearch(r2, &root, record_comp);
    twalk(root, act_walk_tree_two);
    BOOST_REQUIRE_EQUAL(v_walk_tree_two.size(), 4);

    BOOST_REQUIRE_NE(v_walk_tree_two[0].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two[0].what), r1);
    BOOST_CHECK_EQUAL(v_walk_tree_two[0].kind, preorder);
    BOOST_CHECK_EQUAL(v_walk_tree_two[0].level, 0);

    BOOST_REQUIRE_NE(v_walk_tree_two[1].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two[1].what), r2);
    BOOST_CHECK_EQUAL(v_walk_tree_two[1].kind, leaf);
    BOOST_CHECK_EQUAL(v_walk_tree_two[1].level, 1);

    BOOST_REQUIRE_NE(v_walk_tree_two[2].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two[2].what), r1);
    BOOST_CHECK_EQUAL(v_walk_tree_two[2].kind, postorder);
    BOOST_CHECK_EQUAL(v_walk_tree_two[2].level, 0);

    BOOST_REQUIRE_NE(v_walk_tree_two[3].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two[3].what), r1);
    BOOST_CHECK_EQUAL(v_walk_tree_two[3].kind, endorder);
    BOOST_CHECK_EQUAL(v_walk_tree_two[3].level, 0);
}

std::vector<walk_item> v_walk_tree_two_inverse;
static void act_walk_tree_two_inverse(const void *what, VISIT kind, int level) {
    v_walk_tree_two_inverse.emplace_back(what, kind, level);
}

BOOST_AUTO_TEST_CASE(walk_tree_two_inverse) {
    v_walk_tree_two_inverse.clear();
    void *root = NULL;
    record *r1 = new record{1};
    tsearch(r1, &root, record_comp);
    record *r2 = new record{0};
    tsearch(r2, &root, record_comp);
    twalk(root, act_walk_tree_two_inverse);
    BOOST_REQUIRE_EQUAL(v_walk_tree_two_inverse.size(), 4);

    BOOST_REQUIRE_NE(v_walk_tree_two_inverse[0].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two_inverse[0].what), r1);
    BOOST_CHECK_EQUAL(v_walk_tree_two_inverse[0].kind, preorder);
    BOOST_CHECK_EQUAL(v_walk_tree_two_inverse[0].level, 0);

    BOOST_REQUIRE_NE(v_walk_tree_two_inverse[1].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two_inverse[1].what), r1);
    BOOST_CHECK_EQUAL(v_walk_tree_two_inverse[1].kind, postorder);
    BOOST_CHECK_EQUAL(v_walk_tree_two_inverse[1].level, 0);

    BOOST_REQUIRE_NE(v_walk_tree_two_inverse[2].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two_inverse[2].what), r2);
    BOOST_CHECK_EQUAL(v_walk_tree_two_inverse[2].kind, leaf);
    BOOST_CHECK_EQUAL(v_walk_tree_two_inverse[2].level, 1);

    BOOST_REQUIRE_NE(v_walk_tree_two_inverse[3].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two_inverse[3].what), r1);
    BOOST_CHECK_EQUAL(v_walk_tree_two_inverse[3].kind, endorder);
    BOOST_CHECK_EQUAL(v_walk_tree_two_inverse[3].level, 0);
}

std::vector<walk_item> v_walk_tree_two_reverse;
static void act_walk_tree_two_reverse(const void *what, VISIT kind, int level) {
    v_walk_tree_two_reverse.emplace_back(what, kind, level);
}

BOOST_AUTO_TEST_CASE(walk_tree_two_reverse) {
    v_walk_tree_two_reverse.clear();
    void *root = NULL;
    record *r1 = new record{0};
    tsearch(r1, &root, record_comp_reverse);
    record *r2 = new record{1};
    tsearch(r2, &root, record_comp_reverse);
    twalk(root, act_walk_tree_two_reverse);
    BOOST_REQUIRE_EQUAL(v_walk_tree_two_reverse.size(), 4);

    BOOST_REQUIRE_NE(v_walk_tree_two_reverse[0].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two_reverse[0].what), r1);
    BOOST_CHECK_EQUAL(v_walk_tree_two_reverse[0].kind, preorder);
    BOOST_CHECK_EQUAL(v_walk_tree_two_reverse[0].level, 0);

    BOOST_REQUIRE_NE(v_walk_tree_two_reverse[1].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two_reverse[1].what), r1);
    BOOST_CHECK_EQUAL(v_walk_tree_two_reverse[1].kind, postorder);
    BOOST_CHECK_EQUAL(v_walk_tree_two_reverse[1].level, 0);

    BOOST_REQUIRE_NE(v_walk_tree_two_reverse[2].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two_reverse[2].what), r2);
    BOOST_CHECK_EQUAL(v_walk_tree_two_reverse[2].kind, leaf);
    BOOST_CHECK_EQUAL(v_walk_tree_two_reverse[2].level, 1);

    BOOST_REQUIRE_NE(v_walk_tree_two_reverse[3].what, nullptr);
    BOOST_CHECK_EQUAL(*((const struct record **)v_walk_tree_two_reverse[3].what), r1);
    BOOST_CHECK_EQUAL(v_walk_tree_two_reverse[3].kind, endorder);
    BOOST_CHECK_EQUAL(v_walk_tree_two_reverse[3].level, 0);
}

BOOST_AUTO_TEST_SUITE_END()
