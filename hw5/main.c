#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LENGTH 0

typedef struct ThreadArgs {
  int* ptr;
  size_t len;
} ThreadArgs;

void print_array(const int* const ptr, size_t const len);
void quicksort(int* const ptr, const size_t len);
size_t partition(int* const ptr, const size_t len);
void* sort_thread(void* const input);
void merge(const int* const src1,
           const size_t len1,
           const int* const src2,
           const size_t len2,
           int* const dest);
void fill_random(int* const ptr, const size_t len);

void print_array(const int* const ptr, size_t const len) {
  printf("[");
  if (len) {  // `len - 1` will underflow if len is 0
    for (size_t i = 0; i < len - 1; ++i) {
      printf("%i, ", ptr[i]);
    }
    printf("%i", ptr[len - 1]);
  }
  puts("]");
}

void quicksort(int* const ptr, const size_t len) {
  if (len <= 1) {
    return;
  }

  // partition into unsorted halves
  size_t pivot_index = partition(ptr, len);
  // sort first half up to pivot point
  quicksort(ptr, pivot_index);
  // sort second half past pivot point
  quicksort(ptr + pivot_index + 1, len - pivot_index - 1);
}

size_t partition(int* const ptr, const size_t len) {
  // choose last element as pivot
  int pivot = ptr[len - 1];
  size_t i = 0;
  int temp;

  // rearrange elements around pivot point
  for (size_t j = 0; j < len - 1; ++j) {
    if (ptr[j] <= pivot) {
      temp = ptr[i];
      ptr[i++] = ptr[j];
      ptr[j] = temp;
    }
  }

  // move pivot into place
  ptr[len - 1] = ptr[i];
  ptr[i] = pivot;
  // return index of pivot
  return i;
}

void* sort_thread(void* const input) {
  // make sure that `input` points to a type compatible with ThreadArgs!
  quicksort(((ThreadArgs*)input)->ptr, ((ThreadArgs*)input)->len);
  pthread_exit(0);
}

void merge(const int* const src1,
           const size_t len1,
           const int* const src2,
           const size_t len2,
           int* const dest) {
  size_t i1 = 0;
  size_t i2 = 0;
  size_t dest_index = 0;

  while (i1 < len1 && i2 < len2) {
    // choose the element to add to the output array in sorted order
    if (src1[i1] < src2[i2]) {
      dest[dest_index++] = src1[i1++];
    } else {
      dest[dest_index++] = src2[i2++];
    }
  }

  // copy the rest of the unexhausted array into dest
  if (i1 < len1) {
    memcpy(dest + dest_index, src1 + i1, sizeof(int) * (len1 - i1));
  } else {
    memcpy(dest + dest_index, src2 + i2, sizeof(int) * (len2 - i2));
  }
}

void fill_random(int* const ptr, const size_t len) {
  time_t t;
  srand((unsigned)time(&t));
  for (size_t i = 0; i < len; ++i) {
    ptr[i] = rand() % 50;
  }
}

int main() {
  int unsorted[ARRAY_LENGTH];
  fill_random(unsorted, ARRAY_LENGTH);

  int sorted[ARRAY_LENGTH];

  pthread_t t1;
  pthread_t t2;
  int create_result;

  // first half of array
  ThreadArgs args1 = {
      .ptr = unsorted,
      .len = ARRAY_LENGTH / 2,
  };
  // second half of array
  ThreadArgs args2 = {
      .ptr = unsorted + (ARRAY_LENGTH / 2),
      // handle odd `ARRAY_LENGTH`
      .len = (ARRAY_LENGTH + 1) / 2,
  };

  if ((create_result = pthread_create(&t1, NULL, sort_thread, (void*)&args1))) {
    printf("failed to create thread 1: %i", create_result);
    return create_result;
  }

  if ((create_result = pthread_create(&t2, NULL, sort_thread, (void*)&args2))) {
    printf("failed to create thread 2: %i", create_result);
    return create_result;
  }

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  // merge two sorted halves of original array
  merge(unsorted, ARRAY_LENGTH / 2, unsorted + (ARRAY_LENGTH / 2),
        (ARRAY_LENGTH + 1) / 2, sorted);

  print_array(sorted, ARRAY_LENGTH);
}
