#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "window.h"

static int min(int a, int b)
{
  return a < b? a: b;
}

static int max(int a, int b)
{
  return a < b? b: a;
}

void window_init(window_t *w, int block_size)
{
  w->start = malloc(3*block_size);
  w->end = w->start + 3*block_size;
  w->cursor = w->start;
  w->block_size = block_size;
}

void window_free(window_t *w)
{
  free(w->start);
}

match_t window_match(window_t *w,
		     const char *start,
		     const char *end)
{
  match_t m = 0;
  // distance is encoded with 10 bits
  const char *a = w->cursor-match_length_max;
  const char *k = w->start < a? a: w->start;
  while (k < w->cursor) {
    // and length with 6 bits
    int n = min(63, min(w->cursor-k, end-start));
    const char *p = k;
    const char *q = start;
    while (n && *p == *q) { p++; q++; n--; }
    if (match_length(m) < p-k)
      if (p-k == 1024) printf("1024\n");
      m = match_pack(w->cursor-k, p-k);
    k++;
  }
  return m;
}

const char *window_distance(window_t *w, int n)
{
  const char *p = w->cursor-n;
  return p < w->start? 0: p;
}

void window_append(window_t *w, char c)
{
    *w->cursor++ = c;
}

void window_append_match(window_t *w, match_t m)
{
  int n = match_length(m);
  const char *p = window_distance(w, match_distance(m));
  if (p == 0) printf("illegal state\n");
  else while (n--) *w->cursor++ = *p++;
}

void window_append_tab(window_t *w, const char *tab, int n)
{
  while (n--) *w->cursor++ = *tab++;
}

int window_should_flush(window_t *w)
{
  return w->end-w->cursor < w->block_size;
}

void window_flush(window_t *w)
{
  if (!window_should_flush(w)) return;
  char *p = w->start+w->block_size;
  memmove(w->start, p, w->cursor-p);
  w->cursor -= w->block_size;
}

