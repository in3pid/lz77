#ifndef WINDOW_H
#define WINDOW_H

/* 
 * Keep track of last N characters in a sliding window.
 */

typedef struct window {
  char *start;
  char *end;
  char *cursor;
} window_t;

// a HUGE payload for the compressed stream,
// making it several times larger than the original

typedef struct match {
  int distance;
  int length;
} match_t;

// heap
void window_init(window_t *w, int length);
void window_free(window_t *w);
// the longest match of [start, end] in w, set m and return m.length
int window_match(window_t *w, match_t *m, const char *start, const char *end);
// return w.cursor-n or 0 if oob
const char *window_distance(window_t *w, int n);
// advance w.cursor
void window_append(window_t *w, char c);
void window_append_match(window_t *w, const match_t *m);
// slide window forward
void window_slide(window_t *w);
//

#endif
