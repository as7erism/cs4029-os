#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
  pid_t fork_returned_pid;
  int count;
  int return_status;

  fork_returned_pid = fork();

  if (fork_returned_pid < 0) {
    printf("The fork() didn't work! Terminate\n");
    exit(0);
  }

  if (fork_returned_pid != 0) {
    wait(&return_status);  // This line is new
    if (WIFEXITED(return_status))
      printf("\nChild Terminated Normally with a return value of %d\n",
             WEXITSTATUS(return_status));
  }

  for (count = 0; count < 10; count++) {
    if (fork_returned_pid == 0)
      printf("                              child says:  count = %d\n", count);
    else
      printf("parent says: count = %d\n", count);
  }

  printf("\n");

  if (fork_returned_pid == 0)
    exit(10);
  else
    exit(20);
}

// WIFEXITED returns true if the passed wstatus value indicates that the child
// exited normally.
//
// WEXITSTATUS returns the exit status of the passed wstatus value.
//
//
//
// The program we've written exits with a status of 10 if it's a child and 20 if
// it's a parent. We print the child's status, which we obtain via the
// `WIFEXITED` and `WEXITSTATUS` macros, in the program. We print the parent's
// status with the special environment variable "?" in the "echo" command.
