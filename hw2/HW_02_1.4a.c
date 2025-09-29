#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
  int num_children;
  printf("How many children should I make? ");
  // read the number of children to make
  scanf("%i", &num_children);

  int child_pid;
  // loop `num_children` times to make `num_children` children
  for (int i = 0; i < num_children; ++i) {
    child_pid = fork();

    // if i am a child (the return value of fork is 0), i don't want to make any more children, so
    // break
    if (child_pid == 0) {
      break;
    }
  }

  // if i am a child (the return value of fork is 0), i can print immediately
  if (child_pid == 0) {
    printf("Hello from child process %i\n", getpid());
  }
  // otherwise, i have to wait until all my children are done. i will do so by waiting until `wait`
  // returns -1 (error)
  else {
    while (wait(NULL) != -1) {}
    printf("Hello from parent process %i\n", getpid());
  }

}

