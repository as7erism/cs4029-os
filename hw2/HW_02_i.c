#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void SIGINT_signal(int signum) {
  printf("\nI told myself to stop with a SIGINT I sent myself.  Bye Bye\n");
  exit(0);
}

int main(int argc, char** argv, char** envp) {
  int c;

  signal(SIGINT, SIGINT_signal);

  for (c = 0; c < 1000000000;
       c++) {  // The raise system call will send the indicated signal
    // to the process from which it is run.  When the count gets to 5000, it
    // sends itself SIGINT via a kernel system call.
    if (c == 5000)
      raise(SIGINT);
  }
  printf("I should never make it here...\n");
  exit(0);
}
