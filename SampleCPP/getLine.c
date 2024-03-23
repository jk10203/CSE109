#include <stdio.h>
#include <string.h>

/* ssize_t getline(char **lineptr, size_t *n, FILE *stream); */
/* int sscanf(const char *str, const char *format, ...); */

int main() {

  ssize_t numchars = 0;
  char *buffer = NULL;
  size_t length = 0;

  fprintf(stdout, "\nEnter some input (Cntl+D to exit): ");
  while ((numchars = getline(&buffer, &length, stdin)) != -1) {
    /* if numchars == -1, there was an error OR user entered Cntl+D (EOF)
     * if numchars == 1, user just hit Enter key
     * if numchars >1, user entered 1 or more characters followed by Enter
     * if numchars == 0, IDK how this happens!
     */

    fprintf(stderr, "numchars = %zd\n", numchars);
    fprintf(stderr, "length = %zu\n", length);
    fprintf(stderr, "buffer length = %zu\n", strlen(buffer));
    fprintf(stderr, "buffer = \"%s\"\n", buffer);
    fprintf(stderr, "buffer @ 0 = %c\n", buffer[0]); //added this for usefull info

    int numMatches = 0;
    char c = 0;
    long l = 0;

    buffer[numchars - 1] = '\0';
    numMatches = sscanf(buffer, "%c %lu", &c, &l);
    switch (numMatches) {
      case 2: 
        fprintf(stderr, "got %d matches: %c %ld\n", numMatches, c, l);
        break;
      case 1: 
        fprintf(stderr, "got %d match: %c %ld\n", numMatches, c, l);
        break;
      default:
        fprintf(stderr, "got no match or error, return value of sscanf is %d\n", numMatches);
        break;
    }

    fprintf(stdout, "\nEnter some input (Cntl+D to exit): ");
  }
  
  return 0;
}
