// Program HW_02_c.c

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv, char** envp) {
  // use `envp` as the environment variable pointer
  char** env_variable_ptr = envp;

  while (*env_variable_ptr != NULL) {
    printf("%s\n", *env_variable_ptr);
    env_variable_ptr++;
  }
  printf("\n");
}

// `environ` has static duration, so it lives in the data segment, like
// `static`-declared variables.
//
// `envp` is a function parameter, so it is stored in on the stack like other
// local variables.
//
//
//
// A process's copy of environment variable strings are stored in a segment
// above the stack.
