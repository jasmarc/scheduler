#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "heap.h"

int tests_run = 0;
int a[10 +1];

static char * setup() {
    a[1] = 3;
    a[2] = 4;
    a[3] = 1;
    a[4] = 2;
    a[0] = 4;
    return 0;
}

int int_comparision(int a, int b)
{
    if(a < b)
        return -1;
    else if (a == b)
        return 0;
    else
        return 1;
}

static char * test_setup() {
    setup();
    mu_assert("element 1", a[1] == 3);
    mu_assert("element 2", a[2] == 4);
    mu_assert("element 3", a[3] == 1);
    mu_assert("element 4", a[4] == 2);
    return 0;
}

static char * test_relationships() {
    mu_assert("parent of 4", PARENT(4) == 2);
    mu_assert("parent value of 4", a[PARENT(4)] == 4);
    mu_assert("parent of 3", PARENT(3) == 1);
    mu_assert("parent value of 3", a[PARENT(3)] == 3);
    mu_assert("parent of 2", PARENT(2) == 1);
    mu_assert("parent value of 2", a[PARENT(2)] == 3);
    return 0;
}

static char * test_heapify() {
    heapify(a, int_comparision, 1);
    return 0;
}

static char * test_swap() {
    int i, j;
    i = 5; j = 4;
    SWAPINT(i, j);
    mu_assert("swap i", i == 4);
    mu_assert("swap j", j == 5);
    return 0;
}

static char * test_swap_pointers() {
    char *foo_string = "Hello";
    char *bar_string = "world";
    char *temp;
    mu_assert("foo string", strcmp(foo_string, "Hello") == 0);
    mu_assert("bar string", strcmp(bar_string, "world") == 0);
    SWAP(foo_string, bar_string, temp);
    mu_assert("foo string", strcmp(foo_string, "world") == 0);
    mu_assert("bar string", strcmp(bar_string, "Hello") == 0);    
    return 0;
}

static char * test_values_after_heapify() {
    mu_assert("after heapify element 1", a[1] == 4);
    mu_assert("after heapify element 2", a[2] == 3);
    mu_assert("after heapify element 3", a[3] == 1);
    mu_assert("after heapify element 4", a[4] == 2);
    return 0;
}

static char * test_build_heap() {
    setup();
    mu_assert("element 1", a[1] == 3);
    mu_assert("element 2", a[2] == 4);
    mu_assert("element 3", a[3] == 1);
    mu_assert("element 4", a[4] == 2);
    build_heap(a, int_comparision);
    mu_assert("after build_heap element 1", a[1] == 4);
    mu_assert("after build_heap element 2", a[2] == 3);
    mu_assert("after build_heap element 3", a[3] == 1);
    mu_assert("after build_heap element 4", a[4] == 2);
    return 0;
}

static char * test_heap_sort() {
    setup();
    mu_assert("element 1", a[1] == 3);
    mu_assert("element 2", a[2] == 4);
    mu_assert("element 3", a[3] == 1);
    mu_assert("element 4", a[4] == 2);
    heap_sort(a, int_comparision);
    mu_assert("after heap_sort element 1", a[1] == 1);
    mu_assert("after heap_sort element 2", a[2] == 2);
    mu_assert("after heap_sort element 3", a[3] == 3);
    mu_assert("after heap_sort element 4", a[4] == 4);
    return 0;
}

static char * test_heap_extract_max() {
    setup();
    mu_assert("element 1", a[1] == 3);
    mu_assert("element 2", a[2] == 4);
    mu_assert("element 3", a[3] == 1);
    mu_assert("element 4", a[4] == 2);
    build_heap(a, int_comparision);
    mu_assert("heap_extract_max == 4", heap_extract_max(a, int_comparision) == 4);
    mu_assert("heap_extract_max == 3", heap_extract_max(a, int_comparision) == 3);
    mu_assert("heap_extract_max == 2", heap_extract_max(a, int_comparision) == 2);
    mu_assert("heap_extract_max == 1", heap_extract_max(a, int_comparision) == 1);
    mu_assert("heapsize is now zero", HEAPSIZE(a) == 0);
    return 0;
}

static char * test_increment_operator() {
    int i = 5;
    int j = ++i;
    mu_assert("j == 6", j == 6);
    return 0;
}

static char * test_heap_insert() {
    setup();
    build_heap(a, int_comparision);
    heap_insert(a, int_comparision, 10);
    heap_insert(a, int_comparision, 7);
    heap_insert(a, int_comparision, 3);
    mu_assert("heapsize should be 7 after inserts", HEAPSIZE(a) == 7);
    mu_assert("heap_extract_max == 10", heap_extract_max(a, int_comparision) == 10);
    mu_assert("heapsize should be 6", HEAPSIZE(a) == 6);
    mu_assert("heap_extract_max == 7", heap_extract_max(a, int_comparision) == 7);
    mu_assert("heapsize should be 5", HEAPSIZE(a) == 5);
    mu_assert("heap_extract_max == 4", heap_extract_max(a, int_comparision) == 4);
    mu_assert("heapsize should be 4", HEAPSIZE(a) == 4);
    mu_assert("heap_extract_max == 3", heap_extract_max(a, int_comparision) == 3);
    mu_assert("heapsize should be 3", HEAPSIZE(a) == 3);
    mu_assert("heap_extract_max == 3", heap_extract_max(a, int_comparision) == 3);
    mu_assert("heapsize should be 2", HEAPSIZE(a) == 2);
    mu_assert("heap_extract_max == 2", heap_extract_max(a, int_comparision) == 2);
    mu_assert("heapsize should be 1", HEAPSIZE(a) == 1);
    mu_assert("heap_extract_max == 1", heap_extract_max(a, int_comparision) == 1);
    mu_assert("heapsize should be 0", HEAPSIZE(a) == 0);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_setup);
    mu_run_test(test_relationships);
    mu_run_test(test_heapify);
    mu_run_test(test_swap);
    mu_run_test(test_swap_pointers);
    mu_run_test(test_values_after_heapify);
    mu_run_test(test_build_heap);
    mu_run_test(test_heap_sort);
    mu_run_test(test_heap_extract_max);
    mu_run_test(test_heap_extract_max);
    mu_run_test(test_increment_operator);
    mu_run_test(test_heap_insert);
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
