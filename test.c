#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "heap.h"

typedef struct {
    int arrive;
} thing;

int tests_run = 0;
heap *h;

static char * setup() {
    int j;
    h = malloc(sizeof(heap));
    h->size = 4;
    for(j = 1; j <= 4; j++)
        h->a[j] = malloc(sizeof(thing));
    ((thing*)(h->a[1]))->arrive = 3;
    ((thing*)(h->a[2]))->arrive = 4;
    ((thing*)(h->a[3]))->arrive = 1;
    ((thing*)(h->a[4]))->arrive = 2;
    
    return 0;
}

int fcfs_comparision(void *a, void *b)
{
    return (((thing*)(a))->arrive - ((thing*)(b))->arrive);
}

static char * test_setup() {
    setup();
    mu_assert("element 1", ((thing*)(h->a[1]))->arrive == 3);
    mu_assert("element 2", ((thing*)(h->a[2]))->arrive == 4);
    mu_assert("element 3", ((thing*)(h->a[3]))->arrive == 1);
    mu_assert("element 4", ((thing*)(h->a[4]))->arrive == 2);
    return 0;
}

static char * test_relationships() {
    mu_assert("parent of 4", PARENT(4) == 2);
    mu_assert("parent value of 4", ((thing*)(h->a[PARENT(4)]))->arrive == 4);
    mu_assert("parent of 3", PARENT(3) == 1);
    mu_assert("parent value of 3", ((thing*)(h->a[PARENT(3)]))->arrive == 3);
    mu_assert("parent of 2", PARENT(2) == 1);
    mu_assert("parent value of 2", ((thing*)(h->a[PARENT(2)]))->arrive == 3);
    return 0;
}

static char * test_heapify() {
    heapify(h, fcfs_comparision, 1);
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
    mu_assert("after heapify element 1", ((thing*)(h->a[1]))->arrive == 4);
    mu_assert("after heapify element 2", ((thing*)(h->a[2]))->arrive == 3);
    mu_assert("after heapify element 3", ((thing*)(h->a[3]))->arrive == 1);
    mu_assert("after heapify element 4", ((thing*)(h->a[4]))->arrive == 2);
    return 0;
}

static char * test_build_heap() {
    setup();
    mu_assert("element 1", ((thing*)(h->a[1]))->arrive == 3);
    mu_assert("element 2", ((thing*)(h->a[2]))->arrive == 4);
    mu_assert("element 3", ((thing*)(h->a[3]))->arrive == 1);
    mu_assert("element 4", ((thing*)(h->a[4]))->arrive == 2);
    build_heap(h, fcfs_comparision);
    mu_assert("after build_heap element 1", ((thing*)(h->a[1]))->arrive == 4);
    mu_assert("after build_heap element 2", ((thing*)(h->a[2]))->arrive == 3);
    mu_assert("after build_heap element 3", ((thing*)(h->a[3]))->arrive == 1);
    mu_assert("after build_heap element 4", ((thing*)(h->a[4]))->arrive == 2);
    return 0;
}

static char * test_heap_sort() {
    setup();
    mu_assert("element 1", ((thing*)(h->a[1]))->arrive == 3);
    mu_assert("element 2", ((thing*)(h->a[2]))->arrive == 4);
    mu_assert("element 3", ((thing*)(h->a[3]))->arrive == 1);
    mu_assert("element 4", ((thing*)(h->a[4]))->arrive == 2);
    heap_sort(h, fcfs_comparision);
    mu_assert("after heap_sort element 1", ((thing*)(h->a[1]))->arrive == 1);
    mu_assert("after heap_sort element 2", ((thing*)(h->a[2]))->arrive == 2);
    mu_assert("after heap_sort element 3", ((thing*)(h->a[3]))->arrive == 3);
    mu_assert("after heap_sort element 4", ((thing*)(h->a[4]))->arrive == 4);
    return 0;
}

static char * test_heap_extract_max() {
    setup();
    mu_assert("element 1", ((thing*)(h->a[1]))->arrive == 3);
    mu_assert("element 2", ((thing*)(h->a[2]))->arrive == 4);
    mu_assert("element 3", ((thing*)(h->a[3]))->arrive == 1);
    mu_assert("element 4", ((thing*)(h->a[4]))->arrive == 2);
    build_heap(h, fcfs_comparision);
    mu_assert("heap_extract_max == 4", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 4);
    mu_assert("heap_extract_max == 3", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 3);
    mu_assert("heap_extract_max == 2", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 2);
    mu_assert("heap_extract_max == 1", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 1);
    mu_assert("heapsize is now zero", h->size == 0);
    return 0;
}

static char * test_increment_operator() {
    int i = 5;
    int j = ++i;
    mu_assert("j == 6", j == 6);
    return 0;
}

static char * test_heap_insert() {
    thing *fixture1 = malloc(sizeof(thing));
    thing *fixture2 = malloc(sizeof(thing));
    thing *fixture3 = malloc(sizeof(thing));
    fixture1->arrive = 10;
    fixture2->arrive = 7;
    fixture3->arrive = 3;
    setup();
    build_heap(h, fcfs_comparision);
    heap_insert(h, fcfs_comparision, fixture1);
    heap_insert(h, fcfs_comparision, fixture2);
    heap_insert(h, fcfs_comparision, fixture3);
    mu_assert("heapsize should be 7 after inserts", h->size == 7);
    mu_assert("heap_extract_max == 10", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 10);
    mu_assert("heapsize should be 6", h->size == 6);
    mu_assert("heap_extract_max == 7", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 7);
    mu_assert("heapsize should be 5", h->size == 5);
    mu_assert("heap_extract_max == 4", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 4);
    mu_assert("heapsize should be 4", h->size == 4);
    mu_assert("heap_extract_max == 3", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 3);
    mu_assert("heapsize should be 3", h->size == 3);
    mu_assert("heap_extract_max == 3", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 3);
    mu_assert("heapsize should be 2", h->size == 2);
    mu_assert("heap_extract_max == 2", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 2);
    mu_assert("heapsize should be 1", h->size == 1);
    mu_assert("heap_extract_max == 1", ((thing*)(heap_extract_max(h, fcfs_comparision)))->arrive == 1);
    mu_assert("heapsize should be 0", h->size == 0);
    return 0;
}

static char * test_pointers() {
    thing *fixture1 = malloc(sizeof(thing));
    thing *fixture2 = malloc(sizeof(thing));
    //thing *fixture3 = NULL;
    
    fixture1->arrive = 4;
    fixture2->arrive = 5;
    fixture3->arrive = 6;
    
    int *foo;
    int *bar;
    foo = malloc(sizeof(int));
    *foo = 5;
    bar = foo;
    mu_assert("bar == 5", *bar == 5);
    mu_assert("foo == 5", *foo == 5);
    free(bar);
    bar = NULL;
    mu_assert("foo == 5 after free", *foo == 5);
    //mu_assert("bar == 5 after free", *bar == 5);
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
