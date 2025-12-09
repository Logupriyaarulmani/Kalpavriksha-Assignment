/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* maxSlidingWindow(int* nums, int numsSize, int k, int* returnSize) {
    int* res = (int*)malloc((numsSize - k + 1) * sizeof(int));
    int* dq = (int*)malloc(numsSize * sizeof(int)); 
    int left = 0, right = 0, idx = 0;

    for (int i = 0; i < numsSize; i++) {
        while (left < right && nums[dq[right - 1]] < nums[i]) {
            right--;
        }

        dq[right++] = i;

        if (dq[left] <= i - k) {
            left++;
        }

        if (i >= k - 1) {
            res[idx++] = nums[dq[left]];
        }
    }

    *returnSize = idx;
    free(dq);
    return res;
}