// Author: Aster Mayhew

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CHILDREN 5
#define SIGINT_LIMIT 2
#define SLEEP_SECONDS 5

static int sigints_seen = 0;

void handle_sigint(int _) {
  // if this is at least the `SIGINT_LIMIT`th sigint we're handling, exit
  if ((++sigints_seen) >= SIGINT_LIMIT) {
    printf("I am %i and I see the light.\n", getpid());
    exit(0);
  }
  // otherwise, ignore this signal
  else {
    printf("I am %i and it's not my time yet.\n", getpid());
  }
}

void child_main() {
  // only the children should register this handler
  signal(SIGINT, handle_sigint);
  // infinite loop
  while (1) {
  }
}

int main() {
  pid_t children[NUM_CHILDREN];

  // fork `NUM_CHILDREN` times and put the resulting pids in `children`
  for (int i = 0; i < NUM_CHILDREN; ++i) {
    children[i] = fork();

    if (children[i] == -1) {
      puts("Fork failed!\n");
      exit(0);
    }
    // if i am a child, start `child_main`
    else if (children[i] == 0) {
      child_main();
      exit(0);
    }
  }

  for (int _ = 0; _ < SIGINT_LIMIT; ++_) {
    // wait `SLEEP_SECONDS` before sending sigints to children, counting down
    // every second
    fputs("Killing children in ", stdout);
    for (int countdown = SLEEP_SECONDS; countdown > 0; --countdown) {
      printf("%i...\n", countdown);
      sleep(1);
    }

    // send the sigints
    for (int i = 0; i < NUM_CHILDREN; ++i) {
      kill(children[i], SIGINT);
    }
  }
}
