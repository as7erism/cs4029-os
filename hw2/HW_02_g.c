#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
  pid_t fork_returned_pid;

  fork_returned_pid = fork();

  if (fork_returned_pid < 0) {
    printf("The fork() didn't work! Terminate\n");
    exit(0);
  }

  if (fork_returned_pid != 0) {
    printf("The parent will now wait\n");
    wait(NULL);
    printf("The parent is done\n");
  }

  else {
    printf("The child is about to rewrite itself to run new code\n");
    execvp(*(argv + 1), argv + 1);
    printf("this prints only if the previous call fails\n");
  }
}

// This program spawns a child that rewrites itself to execute a specified
// program. That program is obtained by using the parent's first command line
// argument as the program and the rest of its arguments as the child's
// arguments. The name of the child program is the second element of `argv`
// (`*(argv + 1)`), and the arguments are given by passing `argv` starting at
// the second element (`argv + 1`). Thus, running this program with "./a.out ls
// -F" mimics the execution of "ls -F" along with its extra print statements.
//
//
//
//
// Calling this program with no arguments results in a successful execution with
// no external program code being run. Since `argv` ends with an empty string,
// it can be thought of to look like this:
// ["./a.out", ""]
// Our pointer arithmetic passes the second element of `argv` to `execvp`. This
// means that `execvp` is called with the empty string, which is a valid call
// that does nothing.
//
//
//
// "./a.out hoopyfrood" attempts to call `execvp` with "hoopyfrood" as the first
// argument. `execvp` looks in your PATH environment variable for a file to
// execute, and since "hoopyfrood" is not an executable file in my path, it
// fails and thus prints the failure statement.
