int searchInsert(int* nums, int numsSize, int target) {
    int first = 0;
    int last = numsSize - 1;
    int found = -1;
    while (first <= last) {
        int mid = (first + last) / 2;

        if (nums[mid] == target) {
            found = mid;
            return found;
        }
        else if (nums[mid] < target) {
            first = mid + 1;
        }
        else if (nums[mid] > target){
            last = mid - 1;
        }
    }
    return first;
}