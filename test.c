#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "heap.h"

int tests_run = 0;
job *a[10 +1];

static char * setup() {
    int j;
    a[0] = malloc(sizeof(int));
    for(j = 1; j <= 4; j++)
        a[j] = malloc(sizeof(job));
    build_job(a[1], 0, 3, 2, 0, 0, 0);
    build_job(a[2], 1, 4, 5, 0, 0, 0);
    build_job(a[3], 2, 1, 12, 0, 0, 0);
    build_job(a[4], 3, 2, 2, 0, 0, 0);
    HEAPSIZE(a) = 4;
    return 0;
}

int fcfs_comparision(job *a, job *b)
{
    return (a->arrive - b->arrive);
}

static char * test_setup() {
    setup();
    mu_assert("element 1", a[1]->arrive == 3);
    mu_assert("element 2", a[2]->arrive == 4);
    mu_assert("element 3", a[3]->arrive == 1);
    mu_assert("element 4", a[4]->arrive == 2);
    return 0;
}

static char * test_relationships() {
    mu_assert("parent of 4", PARENT(4) == 2);
    mu_assert("parent value of 4", a[PARENT(4)]->arrive == 4);
    mu_assert("parent of 3", PARENT(3) == 1);
    mu_assert("parent value of 3", a[PARENT(3)]->arrive == 3);
    mu_assert("parent of 2", PARENT(2) == 1);
    mu_assert("parent value of 2", a[PARENT(2)]->arrive == 3);
    return 0;
}

static char * test_heapify() {
    heapify(a, fcfs_comparision, 1);
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
    mu_assert("after heapify element 1", a[1]->arrive == 4);
    mu_assert("after heapify element 2", a[2]->arrive == 3);
    mu_assert("after heapify element 3", a[3]->arrive == 1);
    mu_assert("after heapify element 4", a[4]->arrive == 2);
    return 0;
}

static char * test_build_heap() {
    setup();
    mu_assert("element 1", a[1]->arrive == 3);
    mu_assert("element 2", a[2]->arrive == 4);
    mu_assert("element 3", a[3]->arrive == 1);
    mu_assert("element 4", a[4]->arrive == 2);
    build_heap(a, fcfs_comparision);
    mu_assert("after build_heap element 1", a[1]->arrive == 4);
    mu_assert("after build_heap element 2", a[2]->arrive == 3);
    mu_assert("after build_heap element 3", a[3]->arrive == 1);
    mu_assert("after build_heap element 4", a[4]->arrive == 2);
    return 0;
}

static char * test_heap_sort() {
    setup();
    mu_assert("element 1", a[1]->arrive == 3);
    mu_assert("element 2", a[2]->arrive == 4);
    mu_assert("element 3", a[3]->arrive == 1);
    mu_assert("element 4", a[4]->arrive == 2);
    heap_sort(a, fcfs_comparision);
    mu_assert("after heap_sort element 1", a[1]->arrive == 1);
    mu_assert("after heap_sort element 2", a[2]->arrive == 2);
    mu_assert("after heap_sort element 3", a[3]->arrive == 3);
    mu_assert("after heap_sort element 4", a[4]->arrive == 4);
    return 0;
}

static char * test_heap_extract_max() {
    setup();
    mu_assert("element 1", a[1]->arrive == 3);
    mu_assert("element 2", a[2]->arrive == 4);
    mu_assert("element 3", a[3]->arrive == 1);
    mu_assert("element 4", a[4]->arrive == 2);
    build_heap(a, fcfs_comparision);
    mu_assert("heap_extract_max == 4", heap_extract_max(a, fcfs_comparision)->arrive == 4);
    mu_assert("heap_extract_max == 3", heap_extract_max(a, fcfs_comparision)->arrive == 3);
    mu_assert("heap_extract_max == 2", heap_extract_max(a, fcfs_comparision)->arrive == 2);
    mu_assert("heap_extract_max == 1", heap_extract_max(a, fcfs_comparision)->arrive == 1);
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
    job *fixture1 = malloc(sizeof(job));
    job *fixture2 = malloc(sizeof(job));
    job *fixture3 = malloc(sizeof(job));
    build_job(fixture1, 4, 10, 2, 0, 0, 0);
    build_job(fixture2, 5, 7, 2, 0, 0, 0);
    build_job(fixture3, 6, 3, 2, 0, 0, 0);
    setup();
    build_heap(a, fcfs_comparision);
    heap_insert(a, fcfs_comparision, fixture1);
    heap_insert(a, fcfs_comparision, fixture2);
    heap_insert(a, fcfs_comparision, fixture3);
    mu_assert("heapsize should be 7 after inserts", HEAPSIZE(a) == 7);
    mu_assert("heap_extract_max == 10", heap_extract_max(a, fcfs_comparision)->arrive == 10);
    mu_assert("heapsize should be 6", HEAPSIZE(a) == 6);
    mu_assert("heap_extract_max == 7", heap_extract_max(a, fcfs_comparision)->arrive == 7);
    mu_assert("heapsize should be 5", HEAPSIZE(a) == 5);
    mu_assert("heap_extract_max == 4", heap_extract_max(a, fcfs_comparision)->arrive == 4);
    mu_assert("heapsize should be 4", HEAPSIZE(a) == 4);
    mu_assert("heap_extract_max == 3", heap_extract_max(a, fcfs_comparision)->arrive == 3);
    mu_assert("heapsize should be 3", HEAPSIZE(a) == 3);
    mu_assert("heap_extract_max == 3", heap_extract_max(a, fcfs_comparision)->arrive == 3);
    mu_assert("heapsize should be 2", HEAPSIZE(a) == 2);
    mu_assert("heap_extract_max == 2", heap_extract_max(a, fcfs_comparision)->arrive == 2);
    mu_assert("heapsize should be 1", HEAPSIZE(a) == 1);
    mu_assert("heap_extract_max == 1", heap_extract_max(a, fcfs_comparision)->arrive == 1);
    mu_assert("heapsize should be 0", HEAPSIZE(a) == 0);
    return 0;
}

static char * test_pointers() {
    job *fixture1 = malloc(sizeof(job));
    job *fixture2 = malloc(sizeof(job));
    job *fixture3;

    build_job(fixture1, 4, 10, 2, 0, 0, 0);
    build_job(fixture2, 5, 7, 2, 0, 0, 0);
    build_job(fixture3, 6, 3, 2, 0, 0, 0);

    int *foo = 5;
    free(foo);
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
    mu_run_test(test_increment_operator);
    mu_run_test(test_heap_insert);
    mu_run_test(test_pointers);
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
