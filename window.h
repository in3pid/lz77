#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

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

typedef struct match {
  unsigned int distance;   // 0-1023
  unsigned int length;     // 0-1023
} match_t;

// heap
void window_init(window_t *w, int length);
void window_free(window_t *w);
// the longest match of [start, end] in w, set m and return m.length
int window_match(window_t *w, match_t *m, const char *start, const char *end);
// return w.cursor-n or 0 if oob
const char *window_distance(window_t *w, int n);
//
void window_append(window_t *w, char c);
void window_append_match(window_t *w, const match_t *m);
//
int window_should_flush(window_t *w);
void window_flush(window_t *w);

uint16_t match_pack(const match_t *m);
void match_unpack(match_t *m, uint16_t n);

void write_match(int f, match_t *m);
int read_match(int f, match_t *m);

#endif
