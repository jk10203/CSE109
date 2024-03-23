#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("%s: argc is %d\n", argv[0], argc);
  for (int i = 1; i < argc; i++) {
    printf("%s: argv[%d] is '%s'\n", argv[0], i, argv[i]);
  }
  return 0;
}
