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
#include <rbiter.h>

// libraries.
#include <check.h>

int compare (const void* left, const void* right)
{
    return (*(int*)(left) == *(int*)(right)) ? 0 : ((*(int*)(left) < *(int*)(right)) ? -1 : 1);
}

START_TEST (test_it_beg)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);
    struct rbiter  it;

    /* insert nodes */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* try to find first node */
    ck_assert_ptr_eq (it_beg (&it, tree), &val1);

    /* check first node value */
    ck_assert_int_eq (*(int*)it_beg (&it, tree), val1);

    rb_delete (tree);
}
END_TEST

START_TEST (test_it_end)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);
    struct rbiter  it;

    /* insert data */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* try to find last node */
    ck_assert_ptr_eq (it_end (&it, tree), &val5);

    /* check last node value */
    ck_assert_int_eq (*(int*)it_end (&it, tree), val5);

    rb_delete (tree);
}
END_TEST

START_TEST (test_it_next)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);
    struct rbiter  it;

    /* insert nodes */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* iterate threw nodes */
    ck_assert_ptr_eq (it_beg (&it, tree), &val1);
    ck_assert_int_eq (*(int*)it_cur (&it), val1);
    ck_assert_ptr_eq (it_next (&it), &val2);
    ck_assert_int_eq (*(int*)it_cur (&it), val2);
    ck_assert_ptr_eq (it_next (&it), &val3);
    ck_assert_int_eq (*(int*)it_cur (&it), val3);
    ck_assert_ptr_eq (it_next (&it), &val4);
    ck_assert_int_eq (*(int*)it_cur (&it), val4);
    ck_assert_ptr_eq (it_next (&it), &val5);
    ck_assert_int_eq (*(int*)it_cur (&it), val5);
    ck_assert_ptr_eq (it_next (&it), NULL);

    /* remove nodes */
    rb_remove (&val2, tree);
    rb_remove (&val4, tree);

    /* iterate threw nodes again */
    ck_assert_ptr_eq (it_beg (&it, tree), &val1);
    ck_assert_int_eq (*(int*)it_cur (&it), val1);
    ck_assert_ptr_eq (it_next (&it), &val3);
    ck_assert_int_eq (*(int*)it_cur (&it), val3);
    ck_assert_ptr_eq (it_next (&it), &val5);
    ck_assert_int_eq (*(int*)it_cur (&it), val5);
    ck_assert_ptr_eq (it_next (&it), NULL);

    rb_delete (tree);
}
END_TEST

START_TEST (test_it_prev)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);
    struct rbiter  it;

    /* insert nodes */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* iterate threw nodes */
    ck_assert_ptr_eq (it_end (&it, tree), &val5);
    ck_assert_int_eq (*(int*)it_cur (&it), val5);
    ck_assert_ptr_eq (it_prev (&it), &val4);
    ck_assert_int_eq (*(int*)it_cur (&it), val4);
    ck_assert_ptr_eq (it_prev (&it), &val3);
    ck_assert_int_eq (*(int*)it_cur (&it), val3);
    ck_assert_ptr_eq (it_prev (&it), &val2);
    ck_assert_int_eq (*(int*)it_cur (&it), val2);
    ck_assert_ptr_eq (it_prev (&it), &val1);
    ck_assert_int_eq (*(int*)it_cur (&it), val1);
    ck_assert_ptr_eq (it_prev (&it), NULL);

    /* remove nodes */
    rb_remove (&val2, tree);
    rb_remove (&val4, tree);

    /* iterate threw nodes again */
    ck_assert_ptr_eq (it_end (&it, tree), &val5);
    ck_assert_int_eq (*(int*)it_cur (&it), val5);
    ck_assert_ptr_eq (it_prev (&it), &val3);
    ck_assert_int_eq (*(int*)it_cur (&it), val3);
    ck_assert_ptr_eq (it_prev (&it), &val1);
    ck_assert_int_eq (*(int*)it_cur (&it), val1);
    ck_assert_ptr_eq (it_prev (&it), NULL);

    rb_delete (tree);
}
END_TEST

START_TEST (test_it_cur)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);
    struct rbiter  it;

    /* insert data */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* move to first node */
    ck_assert_ptr_eq (it_beg (&it, tree), &val1);

    /* try to find current node */
    ck_assert_ptr_eq (it_cur (&it), &val1);

    /* check current node value */
    ck_assert_int_eq (*(int*)it_cur (&it), val1);

    /* move to last node */
    ck_assert_ptr_eq (it_end (&it, tree), &val5);

    /* try to find current node */
    ck_assert_ptr_eq (it_cur (&it), &val5);

    /* check current node value */
    ck_assert_int_eq (*(int*)it_cur (&it), val5);

    rb_delete (tree);
}
END_TEST

int main (void)
{
    Suite* s = suite_create ("rbiter");
    TCase* core = tcase_create ("core");

    suite_add_tcase (s, core);
    tcase_add_test (core, test_it_beg);
    tcase_add_test (core, test_it_end);
    tcase_add_test (core, test_it_cur);
    tcase_add_test (core, test_it_next);
    tcase_add_test (core, test_it_prev);

    SRunner* runner = srunner_create (s);
    srunner_set_fork_status (runner, CK_NOFORK);
    srunner_run_all (runner, CK_ENV);
    int nf = srunner_ntests_failed (runner);
    srunner_free (runner);

    return nf == 0 ? 0 : 1;
}
