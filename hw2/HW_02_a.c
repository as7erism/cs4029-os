#include <stdio.h>

int main(int argc, char** argv) {
  // argv is an array (pointer) of strings (arrays of chars) whose last element
  // is the empty string. we can pass the string pointed to by argv to printf,
  // incrememnting the pointer each time, until weve reached the empty string.
  while (*argv != 0)
    printf("%s\n", *(argv++));
}
