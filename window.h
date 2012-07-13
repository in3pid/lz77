#ifndef WINDOW_H
#define WINDOW_H

typedef struct window {
  char *start;
  char *end;
  char *cursor;
} window_t;

typedef struct match {
  int length;
  int distance;
} match_t;

void window_init(window_t *, int);
void window_free(window_t *);
void window_moveback(window_t *);
void window_append(window_t *, char);
int window_match(window_t *, match_t *, const char *, const char *);
const char *window_distance(window_t *, int);

#endif
