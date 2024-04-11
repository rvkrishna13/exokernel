#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define ITERATIONS 100000

int main() {
  int pipe1[2], pipe2[2];
  char byte = 'X';

  // Create pipes
  if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
    fprintf(2, "pipe failed\n");
    exit(0);
  }

  // Fork child process
  int pid = fork();
  if (pid < 0) {
    fprintf(2, "fork failed\n");
    exit(0);
  }

  if (pid == 0) {
    // Child process
    close(pipe1[1]);
    close(pipe2[0]);

    for (int i = 0; i < ITERATIONS; i++) {
      read(pipe1[0], &byte, 1);
      write(pipe2[1], &byte, 1);
    }

    close(pipe1[0]);
    close(pipe2[1]);
    exit(0);
  } else {
    // Parent process
    close(pipe1[0]);
    close(pipe2[1]);

    int start_ticks = uptime();

    for (int i = 0; i < ITERATIONS; i++) {
      write(pipe1[1], &byte, 1);
      read(pipe2[0], &byte, 1);
    }

    int end_ticks = uptime();

    close(pipe1[1]);
    close(pipe2[0]);

    wait(0);

    int elapsed_ticks = end_ticks - start_ticks;
    int exchanges_per_second = (ITERATIONS * 1000) / elapsed_ticks;

    fprintf(1, "Exchanges per second: %d\n", exchanges_per_second);
    exit(0);
  }
}