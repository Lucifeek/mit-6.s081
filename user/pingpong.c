#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // create pipe
  int fds[2];
  if(pipe(fds) != 0){
    printf("pipe() failed\n");
    exit(1);
  }
  
  // fork child process
  int pid;
  
  pid = fork();
  if (pid == 0) {
    // child process
    // fds[1] for write, fds[0] for read
    char data;
    while(read(fds[0], &data, 1) != 1);
    close(fds[0]);
    printf("%d: received ping\n", getpid());
    write(fds[1], &data, 1);
    close(fds[1]);
  } else if(pid > 0) {
    // fds[1] for write, fds[0] for read
    char data;
    write(fds[1], "l", 1);
    close(fds[1]);
    while(read(fds[0], &data, 1) != 1);
    close(fds[0]);
    printf("%d: received pong\n", getpid());
  } else {
    //error
    printf("fork() failed\n");
    exit(1);
  }
  exit(0);
}
