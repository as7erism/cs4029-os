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

  if (fork_returned_pid != 0)
    wait(NULL);  // This line is new

  for (count = 0; count < 1000; count++) {
    if (fork_returned_pid == 0)
      printf("                              child says:  count = %d\n", count);
    else
      printf("parent says: count = %d\n", count);
  }
}

// In "HW_02_d.c", the parent and child (mostly) alternate prints. However, "HW_02_e.c" always
// prints every child count in order before printing every parent count. This is because the
// parent waits for its child to terminate via `wait` before proceeding to count.
//
//
//
// `wait` returns -1 and the process continues immediately.
//
//
//
//
