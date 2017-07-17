#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    cs->stack = calloc(k+2, sizeof(int));
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

void free_state(comb_state *cs)
{
    free(cs->stack);
}

/** Tests if the die has the correct average */
int sum_test(int *nums, int m, int expected)
{
    int total = 0;
    for(int i = 0; i < m; i++)
        total += nums[i];

    return total == expected;
}

void foo_helper(int *nums, int num_dice, int num_sides, int expected, int d)
{
    // Are we on the final die?
    if(d == num_dice - 1)
    {
        for(int i = 0; i < num_dice; i++)
        {
            for(int j = 0; j < num_sides; j++)
            {
                printf("%d ", nums[i * num_sides + j]);
            }
            printf("| ");
        }
        printf("\n");
        return;
    }

    // Find the point where we left off
    int *rest = nums + d * num_sides;
    int num_rest = (num_dice - d) * num_sides;

    // Copy the state of the array
    int *save = malloc(num_rest * sizeof(int));
    memcpy(save, rest, num_rest * sizeof(int));

    // Start generating dice!
    comb_state cs;
    init_state(&cs, num_rest, num_sides);

    do {
        if(sum_test(rest, num_sides, expected))
        {
            foo_helper(nums, num_dice, num_sides, expected, d+1);
        }
    } while(next_comb(&cs, rest));

    free_state(&cs);

    // Restore the state of the array
    memcpy(rest, save, num_rest * sizeof(int));
    free(save);
}

void foo(int num_dice, int num_sides)
{
    int total_sides = num_dice * num_sides;

    int *nums = malloc(total_sides * sizeof(int));
    for(int i = 0; i < total_sides; i++)
        nums[i] = i + 1;

    // Compute the expected sum-of-faces on each die
    int t1 = num_sides * (total_sides + 1);
    if(t1 & 1) // if expected would be non-integer
        return;
    int expected = t1 / 2;

    // Start generating dice!
    foo_helper(nums, num_dice, num_sides, expected, 0);

    free(nums);
}

int main()
{
    foo(3, 3);
}