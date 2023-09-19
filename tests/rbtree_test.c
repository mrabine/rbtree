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

// libraries.
#include <check.h>

int compare (const void* left, const void* right)
{
    return (*(int*)(left) == *(int*)(right)) ? 0 : ((*(int*)(left) < *(int*)(right)) ? -1 : 1);
}

START_TEST (test_rb_new)
{
    struct rbtree* tree = rb_new (compare, NULL);

    ck_assert_ptr_ne (tree, NULL);

    rb_delete (tree);
}
END_TEST

START_TEST (test_rb_insert)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);

    /* try to insert nodes */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* try to insert nodes again */
    ck_assert_ptr_eq (rb_insert (&val1, tree), NULL);
    ck_assert_ptr_eq (rb_insert (&val2, tree), NULL);
    ck_assert_ptr_eq (rb_insert (&val3, tree), NULL);
    ck_assert_ptr_eq (rb_insert (&val4, tree), NULL);
    ck_assert_ptr_eq (rb_insert (&val5, tree), NULL);

    rb_delete (tree);
}
END_TEST

START_TEST (test_rb_find)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);

    /* try to find nodes */
    ck_assert_ptr_eq (rb_find (&val1, tree), NULL);
    ck_assert_ptr_eq (rb_find (&val2, tree), NULL);
    ck_assert_ptr_eq (rb_find (&val3, tree), NULL);
    ck_assert_ptr_eq (rb_find (&val4, tree), NULL);
    ck_assert_ptr_eq (rb_find (&val5, tree), NULL);

    /* insert nodes */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* try to find nodes again */
    ck_assert_ptr_eq (rb_find (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_find (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_find (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_find (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_find (&val5, tree), &val5);

    /* check nodes */
    ck_assert_int_eq (*(int*)rb_find (&val1, tree), 1);
    ck_assert_int_eq (*(int*)rb_find (&val2, tree), 2);
    ck_assert_int_eq (*(int*)rb_find (&val3, tree), 3);
    ck_assert_int_eq (*(int*)rb_find (&val4, tree), 4);
    ck_assert_int_eq (*(int*)rb_find (&val5, tree), 5);

    /* remove nodes */
    rb_remove (&val1, tree);
    rb_remove (&val2, tree);
    rb_remove (&val3, tree);
    rb_remove (&val4, tree);
    rb_remove (&val5, tree);

    /* try to find nodes again */
    ck_assert_ptr_eq (rb_find (&val1, tree), NULL);
    ck_assert_ptr_eq (rb_find (&val2, tree), NULL);
    ck_assert_ptr_eq (rb_find (&val3, tree), NULL);
    ck_assert_ptr_eq (rb_find (&val4, tree), NULL);
    ck_assert_ptr_eq (rb_find (&val5, tree), NULL);

    rb_delete (tree);
}
END_TEST

START_TEST (test_rb_remove)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);

    /* check number of nodes */
    ck_assert_int_eq (rb_size (tree), 0);

    /* insert nodes */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* check number of nodes */
    ck_assert_int_eq (rb_size (tree), 5);

    /* remove nodes */
    rb_remove (&val1, tree);
    rb_remove (&val2, tree);
    rb_remove (&val3, tree);
    rb_remove (&val4, tree);
    rb_remove (&val5, tree);

    /* check number of nodes */
    ck_assert_int_eq (rb_size (tree), 0);

    rb_delete (tree);
}
END_TEST

START_TEST (test_rb_size)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);

    /* check number of nodes */
    ck_assert_int_eq (rb_size (tree), 0);

    /* insert nodes */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* check number of nodes */
    ck_assert_int_eq (rb_size (tree), 5);

    /* erase nodes */
    rb_remove (&val1, tree);
    rb_remove (&val2, tree);

    /* check number of nodes */
    ck_assert_int_eq (rb_size (tree), 3);

    rb_delete (tree);
}
END_TEST

START_TEST (test_rb_empty)
{
    int val1 = 1, val2 = 2, val3 = 3, val4 = 4, val5 = 5;
    struct rbtree* tree = rb_new (compare, NULL);

    /* check if empty */
    ck_assert (rb_empty (tree));

    /* insert nodes */
    ck_assert_ptr_eq (rb_insert (&val1, tree), &val1);
    ck_assert_ptr_eq (rb_insert (&val2, tree), &val2);
    ck_assert_ptr_eq (rb_insert (&val3, tree), &val3);
    ck_assert_ptr_eq (rb_insert (&val4, tree), &val4);
    ck_assert_ptr_eq (rb_insert (&val5, tree), &val5);

    /* check if empty */
    ck_assert (!rb_empty (tree));

    /* erase nodes */
    rb_remove (&val1, tree);
    rb_remove (&val2, tree);
    rb_remove (&val3, tree);

    /* check if empty */
    ck_assert (!rb_empty (tree));

    /* erase nodes */
    rb_remove (&val4, tree);
    rb_remove (&val5, tree);

    /* check if empty */
    ck_assert (rb_empty (tree));

    rb_delete (tree);
}
END_TEST

int main (void)
{
    Suite* s = suite_create ("rbtree");
    TCase* core = tcase_create ("core");

    suite_add_tcase (s, core);
    tcase_add_test (core, test_rb_new);
    tcase_add_test (core, test_rb_insert);
    tcase_add_test (core, test_rb_find);
    tcase_add_test (core, test_rb_remove);
    tcase_add_test (core, test_rb_size);
    tcase_add_test (core, test_rb_empty);

    SRunner* runner = srunner_create (s);
    srunner_set_fork_status (runner, CK_NOFORK);
    srunner_run_all (runner, CK_ENV);
    int nf = srunner_ntests_failed (runner);
    srunner_free (runner);

    return nf == 0 ? 0 : 1;
}
