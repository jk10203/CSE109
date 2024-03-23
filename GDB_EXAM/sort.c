#include <stdio.h>   /* for fprintf, etc., functions */
#include <stdlib.h>  /* for exit function */

void sort(int *, int *);
int getInt(void);
 
int main() {
  int x = getInt();
  int y = getInt();
  fprintf(stdout, "Before sort: x = %d, y = %d\n", x, y); 
  sort(&x, &y);
  fprintf(stdout, "After sort:  x = %d, y = %d\n", x, y);
  return 0;
}

void sort(int *a, int *b) {
  if (*a > *b) {
    int tmp = *a;  /* assign a to tmp */
    *a = *b;       /* assign b to a */
    *b = tmp;      /* assign tmp to b */
  }
  return;
}

int getInt() {
  ssize_t numchars;     /* to hold numchars read from getline, can be -1 if EOF (Cntl+D) is entered or error */
  size_t length;        /* to hold the length of the line read from getline */
  char *line = NULL;    /* to hold line read for favorite integer */
  int number;           /* to hold input */

  do {   /* read in lines until non-empty line is entered which contains an integer in the beginning of the line */
    fprintf(stdout, "Enter an integer (Cntl+D to exit): ");
    while ((numchars = getline(&line, &length, stdin)) <= 1) {
      if (numchars == -1) 
        exit(0);
      else  /* encountered empty line */
        fprintf(stdout, "Enter an integer (Cntl+D to exit): ");
    }
  } while (sscanf(line, "%d", &number) != 1);
  if (line) free(line);

  return number;
}
