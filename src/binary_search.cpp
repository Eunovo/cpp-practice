#include <cmath>
#include <vector>

#include "utils.h"

// Binary search is a search algorithm that finds the position of a target value within a sorted array.
int binary_search(std::vector<int>& arr, int target) {
  int left = 0;
  int high = arr.size() - 1;
  int mid;
  while (left <= high) {
    mid = floor((left + high) / 2);
    if (arr[mid] == target) return mid;
    if (arr[mid] < target) left = mid + 1;
    else high = mid - 1;
  }

  return -1;
}

// Test Binary Search
void test_binary_search() {
  LOG("INFO", "Testing Binary Search Algorithm");
  {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int target = 5;
    int expected = 4;
    int result = binary_search(arr, target);
    IS_TRUE(result == expected);
  }
  {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int target = 5;
    int expected = 4;
    int result = binary_search(arr, target);
    IS_TRUE(result == expected);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int target = 9;
    int expected = 8;
    int result = binary_search(arr, target);
    IS_TRUE(result == expected);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int target = 10;
    int expected = 9;
    int result = binary_search(arr, target);
    IS_TRUE(result == expected);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int target = 1;
    int expected = 0;
    int result = binary_search(arr, target);
    IS_TRUE(result == expected);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int target = 11;
    int expected = -1;
    int result = binary_search(arr, target);
    IS_TRUE(result == expected);
  }

  {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int target = 0;
    int expected = -1;
    int result = binary_search(arr, target);
    IS_TRUE(result == expected);
  }

  LOG("INFO", "Testing Complete!");
}
