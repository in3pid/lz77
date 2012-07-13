#ifndef WINDOW_H
#define WINDOW_H

typedef struct window {
  char *buffer;
  int length; // even
  int position;
} window_t;

void window_init(window_t *, int);
void window_free(window_t *);
void window_copyback(window_t *);
int window_match(window_t *, match_t *, const char *, int);

#endif
