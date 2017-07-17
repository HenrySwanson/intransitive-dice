#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/** Stores the information needed to resume generating combinations. */
typedef struct
{
    int n, k; // n choose k
    int d; // depth in the stack
    int *stack; // stores the number of elements on the right that we are
                // currently allowed to select.
                // this is always (non-strictly) decreasing
} comb_state;

/** Swaps two elements of an array */
void swap(int *arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

/** Initializes a comb_state for choosing k elements out of n. */
void init_state(comb_state *cs, int n, int k)
{
    cs->n = n;
    cs->k = k;
    cs->d = 0;
    cs->stack = calloc(k+1, sizeof(int));
    cs->stack[0] = n-k;
}

/** Generates the next combination. Returns 1 on success, 0 when exhausted. */
int next_comb(comb_state *cs, int *arr)
{
    // This is a transformation of the following recursive algorithm:
    // void gen_combs(int *arr, int k, int n, int d)
    // {
    //     if(k == d)
    //         return;
    //     for(int i = 0; i < n-k; i++)
    //     {
    //         swap(arr, d, k+i)
    //         gen_combs(arr, k, k+i+1, d+1)
    //     }
    // }

    // Base case (bottoming out the stack)
    if(cs->k == cs->d)
        cs->d--;

    // Corresponds to the unwinding of the for loops
    while(cs->stack[cs->d + 1] == cs->stack[cs->d])
    {
        if(cs->d == 0)
            return 0;
        cs->d--;
    }

    // Edits the array, giving a new combination
    int i = cs->stack[cs->d + 1];
    swap(arr, cs->d, cs->k + i);

    // Corresponds to the recursive call
    cs->stack[cs->d + 1]++;
    cs->stack[cs->d + 2] = 0;
    cs->d++;

    return 1;
}

/** Tests if the die has the correct average */
int sum_test(int *nums, int m, int expected)
{
    int total = 0;
    for(int i = 0; i < m; i++)
        total += nums[i];

    return total == expected;
}

int main()
{
    int n = 6;
    int k = 3;

    int nums [6] = {1, 2, 3, 4, 5, 6};
    comb_state cs;
    init_state(&cs, 6, 3);

    do {

        printf("CS: d=%d, stack=[", cs.d);
        for(int i = 0; i < k+1; i++)
            printf("%d ", cs.stack[i]);
        printf("\b] \n");

        for(int i = 0; i < k; i++)
            printf("%d ", nums[i]);
        printf("| ");
        for(int i = 0; i < n-k; i++)
            printf("%d ", nums[k+i]);
        printf("\n");
    } while(next_comb(&cs, nums));

    printf("CS: d=%d, stack=[", cs.d);
    for(int i = 0; i < k+1; i++)
        printf("%d ", cs.stack[i]);
    printf("\b] \n");
}