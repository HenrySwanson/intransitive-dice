#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int nums [6] = {1, 2, 3, 4, 5, 6};

void foo(int *arr, int a, int b)
{
    for(int i = 0; i < 6; i++)
        printf("%d ", nums[i]);
    printf("\n");

    if(a == 0)
        return;
    for(int i = 0; i < b; i++)
    {
        int temp = arr[0];
        arr[0] = arr[a+i];
        arr[a+i] = temp;
        foo(arr+1, a-1, i+1);
    }
}

void foo2(int *arr, int a, int b, int d)
{
    for(int i = 0; i < 6; i++)
        printf("%d ", arr[i]);
    printf("\n");

    if(a == d)
        return;
    for(int i = 0; i < b; i++)
    {
        int temp = arr[d];
        arr[d] = arr[a+i];
        arr[a+i] = temp;
        foo2(arr, a, i+1, d+1);
    }
}

void swap(int *arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void bar(int *arr, int a, int b)
{
    int stack [b];
    for(int i = 0; i < b; i++)
        stack[i] = 0;
    stack[0] = b;

    int d = 0;

    while(1) {
        for(int i = 0; i < 6; i++)
            printf("%d ", arr[i]);
        printf("\n");

        if (d == -1)
            return;

        if(a == d)
            d--;

        int i = stack[d+1];

        if(i < stack[d])
        {
            swap(arr, d, a + i);
            stack[d+1]++;
            stack[d+2] = 0;
            d++;
        }
        else
        {
            d--;
        }
    }
}

void bar2(int *arr, int n, int k)
{
    int stack [k+1];
    for(int i = 0; i < k+1; i++)
        stack[i] = 0;
    stack[0] = n-k;

    int d = 0;

    while(1) {
        for(int i = 0; i < k; i++)
            printf("%d ", arr[i]);
        printf("| ");
        for(int i = 0; i < n-k; i++)
            printf("%d ", arr[k+i]);
        printf("\n");

        if(k == d)
            d--;

        while(stack[d+1] == stack[d])
        {
            d--;
            if(d == -1)
                return;
        }

        int i = stack[d+1];

        swap(arr, d, k + i);
        stack[d+1]++;
        stack[d+2] = 0;
        d++;
    }
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
    //foo2(nums, 3, 3, 0);
    bar2(nums, 6, 3);
}