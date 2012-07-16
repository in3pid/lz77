#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>
#include "match.h"

/* 
 * Keep track of last N characters in a sliding window.
 */

typedef struct window {
  char *start;
  char *end;
  char *cursor;
  int block_size;
} window_t;

// repeating strings <=> <distance, length>
// matches from the history window

#define BLOCK_SIZE 1024

// heap
void window_init(window_t *w, int length);
void window_free(window_t *w);
// the longest match of [start, end] in w
match_t window_match(window_t *w, const char *start, const char *end);
// return w.cursor-n or 0 if oob
const char *window_distance(window_t *w, int n);
//
void window_append(window_t *w, char c);
void window_append_match(window_t *w, match_t m);
void window_append_tab(window_t *w, const char *tab, int n);
//
int window_should_flush(window_t *w);
void window_flush(window_t *w);

#endif
