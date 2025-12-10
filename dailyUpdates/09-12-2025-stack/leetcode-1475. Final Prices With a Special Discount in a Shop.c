/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* finalPrices(int* prices, int pricesSize, int* returnSize) {
    *returnSize = pricesSize;
    int* stack = (int*) malloc (pricesSize * sizeof(int)), stackSize = 0;

    for (int i = 0; i < pricesSize; i++) {
        while (stackSize && prices[stack[stackSize - 1]] >= prices[i]) {
            prices[stack[--stackSize]] -= prices[i];
        }
        stack[stackSize] = i;
        ++stackSize;
    }
    free(stack);
    return prices;
}