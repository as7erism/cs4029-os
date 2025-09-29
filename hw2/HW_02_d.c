#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
  pid_t fork_returned_pid;
  int count;

  fork_returned_pid = fork();

  if (fork_returned_pid < 0) {
    printf("The fork() didn't work! Terminate\n");
    exit(0);
  }

  for (count = 0; count < 1000; count++) {
    if (fork_returned_pid == 0)
      printf("                              child says:  count = %d\n", count);
    else
      printf("parent says: count = %d\n", count);
  }
}

// The child will pick up execution directly after the `fork` call, as though it
// had called it itself.
//
//
//
// `fork` will provide the child with a return value of 0, signaling that it's a
// clone.
//
//
//
// Though theoretically the parent and the child should both begin executing
// right after the `fork` call, the parent starts printing to the screen well
// before the child does. This could be because it takes some time to set up the
// memory and execution state of the child process, while the parent process is
// already set up and ready to go immediately after `fork`ing.
