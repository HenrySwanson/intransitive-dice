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

/** Compares two dice against each other. */
int compete(int *die1, int *die2, int num_sides)
{
    int total = 0;

    for(int i = 0; i < num_sides; i++)
    {
        for(int j = 0; j < num_sides; j++)
        {
            if(die1[i] > die2[j])
                total++;
        }
    }

    return total;
}

/** Tests how intransitive the given set of dice are. */
int intrans_score(int *nums, int num_dice, int num_sides)
{
    // Here, we think of it as a game, where Alice picks a die first,
    // and Bob picks his die in response. The set is intransitive if
    // Bob can always pick a die to beat Alice.

    int alices_min = num_sides * num_sides;
    for(int a = 0; a < num_dice; a++)
    {
        int bobs_max = 0;
        for(int b = 0; b < num_dice; b++)
        {
            // They can't pick the same die
            if(a == b)
                continue;

            // Get dice
            int *a_die = nums + a * num_sides;
            int *b_die = nums + b * num_sides;

            // Bob wants to maximize his score
            int score = compete(b_die, a_die, num_sides);
            if(score > bobs_max)
                bobs_max = score;
        }

        // Alice wants to minimize her score
        if(bobs_max < alices_min)
            alices_min = bobs_max;
    }

    return alices_min;
}

void output_die(int *nums, int num_dice, int num_sides)
{
    printf("|");
    for(int i = 0; i < num_dice; i++)
    {
        for (int j = 0; j < num_sides; j++)
        {
            printf(" %d", nums[i * num_sides + j]);
        }
        printf(" |");
    }
    printf("\n");
}

void gd_helper(int *nums, int num_dice, int num_sides, int expected, int d)
{
    // Are we on the final die? Then we don't have a choice.
    // Fortunately, if all the other sum-tests worked out, ours must also
    // be correct.
    if(d == num_dice - 1)
    {
        int score = intrans_score(nums, num_dice, num_sides);

        if(score * 2 > num_sides * num_sides)
        {
            output_die(nums, num_dice, num_sides);
            printf("score: %d\n", score);
        }
        return;
    }

    // Find the point where we left off
    int *rest = nums + d * num_sides;
    int num_rest = (num_dice - d) * num_sides;

    // Copy the state of the array
    int save [num_rest];
    memcpy(save, rest, num_rest * sizeof(int));

    // Find the minimum element
    int min_idx = 0;
    for(int i = 0; i < num_rest; i++)
    {
        if(rest[i] < rest[min_idx])
        {
            min_idx = i;
        }
    }

    // Force the min elt to be part of our die.
    // This prevents us from generating duplicate arrangements of dice.
    swap(rest, 0, min_idx);

    // Start generating dice!
    // Note that we're only choosing num_sides - 1 sides, because we're
    // forcing ourselves to pick the minimum element.
    comb_state cs;
    init_state(&cs, num_rest-1, num_sides-1);

    do {
        if(sum_test(rest, num_sides, expected))
        {
            gd_helper(nums, num_dice, num_sides, expected, d+1);
        }
    } while(next_comb(&cs, rest+1));

    free_state(&cs);

    // Restore the state of the array
    memcpy(rest, save, num_rest * sizeof(int));
}

void gen_dice(int num_dice, int num_sides)
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
    gd_helper(nums, num_dice, num_sides, expected, 0);

    free(nums);
}

int main()
{
    gen_dice(5, 3);
}