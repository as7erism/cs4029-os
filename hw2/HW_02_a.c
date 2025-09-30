#include <stdio.h>

int main(int argc, char** argv) {
  // argv is an array (pointer) of strings (arrays of chars) whose last element
  // is the empty string. until weve reached the empty string, we can pass the
  // string pointed to by argv to printf, incrementing the pointer each time.
  while (*argv != 0)
    printf("%s\n", *(argv++));
}
