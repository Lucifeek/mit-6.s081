#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int main(int argc, char* argv[])
{
  char buf[512];
  char *args[MAXARG];
  for (int i = 1; i < argc; ++i) {
    args[i-1] = argv[i];
  }
  char* p = buf;
  while (read(0, p, 1) > 0) {
    if (*p == '\n') {
      *p = 0;
      if (fork() == 0) {
        args[argc-1] = buf;
        exec(args[0], args);
        exit(0);
      } else {
        wait(0);
      }
      p = buf;
    } else {
      ++p;
    }
  }
  exit(0);
}
