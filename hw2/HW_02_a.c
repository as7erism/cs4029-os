#include <stdio.h>

int main(int argc, char** argv) {
  int arg_count = 0;
  while (arg_count < argc) {
    // `argv` is a pointer to an array of pointers to arrays of chars
    // ("strings"). pass `printf` the pointer to the `arg_count`st string by
    // using `arg_count` as an offset for `argv` pointer.
    printf("%s\n", *(argv + arg_count));

    // go to the next argument
    ++arg_count;
  }
}
