#include <stddef.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LENGTH 51

typedef struct ThreadArgs {
  int* ptr;
  size_t len;
} ThreadArgs;

void print_arr(int* ptr, size_t len);
void quicksort(int* ptr, size_t len);
size_t partition(int* ptr, size_t len);
void* sort_thread(void* input);
void merge(int* src1, size_t len1, int* src2, size_t len2, int* dest);
void fill_random(int* ptr, size_t len);

void print_arr(int* ptr, size_t len) {
  printf("[");
  for (size_t i = 0; i < len - 1; ++i) {
    printf("%i, ", ptr[i]);
  }
  printf("%i", ptr[len - 1]);
  puts("]");
}

void quicksort(int* ptr, size_t len) {
  if (len <= 1) {
    return;
  }

  size_t pivot_index = partition(ptr, len);
  quicksort(ptr, pivot_index);
  quicksort(ptr + pivot_index + 1, len - pivot_index - 1);
}

size_t partition(int* ptr, size_t len) {
  int pivot = ptr[len - 1];
  size_t i = 0;
  int temp;

  for (size_t j = 0; j < len - 1; ++j) {
    if (ptr[j] <= pivot) {
      temp = ptr[i];
      ptr[i] = ptr[j];
      ptr[j] = temp;
      ++i;
    }
  }

  ptr[len - 1] = ptr[i];
  ptr[i] = pivot;
  return i;
}

void* sort_thread(void* input) {
  quicksort(((ThreadArgs*)input)->ptr, ((ThreadArgs*)input)->len);
  pthread_exit(0);
}

void merge(int* src1, size_t len1, int* src2, size_t len2, int* dest) {
  size_t i1 = 0;
  size_t i2 = 0;
  size_t dest_index = 0;

  while (i1 < len1 && i2 < len2) {
    if (src1[i1] < src2[i2]) {
      dest[dest_index++] = src1[i1++];
    } else {
      dest[dest_index++] = src2[i2++];
    }
  }

  if (i1 < len1) {
    memcpy(dest + dest_index, src1 + i1, sizeof(int) * (len1 - i1));
  } else {
    memcpy(dest + dest_index, src2 + i2, sizeof(int) * (len2 - i2));
  }
}

void fill_random(int* ptr, size_t len) {
  time_t t;
  srand((unsigned)time(&t));
  for (size_t i = 0; i < len; ++i) {
    ptr[i] = rand() % 50;
  }
}

int main() {
  int vals[ARRAY_LENGTH];
  fill_random(vals, ARRAY_LENGTH);

  int merged[ARRAY_LENGTH];

  pthread_t t1;
  pthread_t t2;

  ThreadArgs args1 = {
    .ptr = vals,
    .len = ARRAY_LENGTH / 2,
  };
  ThreadArgs args2 = {
    .ptr = vals + (ARRAY_LENGTH / 2),
    .len = (ARRAY_LENGTH + 1) / 2,
  };

  if (pthread_create(&t1, NULL, sort_thread, (void*)&args1))
    printf("failed to create thread 1");

  if (pthread_create(&t2, NULL, sort_thread, (void*)&args2))
    printf("failed to create thread 2");

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  merge(vals, ARRAY_LENGTH / 2, vals + (ARRAY_LENGTH / 2), (ARRAY_LENGTH + 1) / 2, merged);

  print_arr(merged, ARRAY_LENGTH);
} 
