#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void func(int);

int
main(int argc, char *argv[])
{
  // create pipe
  int fds[2];
  if (pipe(fds) != 0) 
  {
    printf("pipe() failed\n");
    exit(1);
  }

  // fork child process
  int pid;
  pid = fork();
  if (pid > 0) 
  {
    close(fds[0]);
    // fds[0] for read, fds[1] for write
    for (int i = 2; i < 35; ++i)
    {
      write(fds[1], &i, sizeof(int));
    }
    close(fds[1]);
    wait(0);
  }
  else if (pid == 0) 
  {
    // child process
    close(fds[1]);
    func(fds[0]);
  } 
  else 
  {
    //error
    printf("fork() failed - %d\n", pid);
    exit(1);
  }
  exit(0);
}

void 
func(int fd) 
{
  int p;
  if(read(fd, &p, 4) > 0)  // sizeof int is 4
  {
    printf("prime %d\n", p);

    int fds[2];
    if (pipe(fds) != 0)
    {
      printf("pipe() failed\n");
      exit(1);
    }

    int pid = fork();
    if (pid > 0)
    {
      close(fds[0]);
      // read from fd, wirte to fds[1]
      int data;
      while (read(fd, &data, sizeof(int)) > 0)
      {
        if (data % p != 0)
        {
          write(fds[1], &data, 4);
        }
      }
      close(fd);
      close(fds[1]);
      wait(0);
    }
    else if (pid == 0)
    {
      close(fds[1]);
      func(fds[0]);
    }
    else
    {
      //error
      printf("fork() failed\n");
      exit(1);
    }
  }
}
