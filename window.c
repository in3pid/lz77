#include <stdlib.h>
#include <string.h>
#include "window.h"

int min(int a, int b)
{
  return a < b? a: b;
}

int max(int a, int b)
{
  return a < b? b: a;
}

void window_init(window_t *w, int length)
{
  w->start = malloc(length);
  w->end = w->start+length;
  w->cursor = w->start;
  memset(w->start, 0, length);
}

void window_free(window_t *w)
{
  free(w->start);
}

void window_append(window_t *w, char c)
{
    *w->cursor++ = c;
    window_moveback(w);
}

void window_moveback(window_t *w)
{
  if (w->cursor < w->end) return;
  memmove(w->start, w->start+1, w->end-w->start);
  w->cursor--;
}

int window_match(window_t *w,
		 match_t *match,
		 const char *start,
		 const char *end)
{
  const char *k = w->start;
  memset(match, 0, sizeof match);
  while (k < w->cursor) {
    int n = min(w->cursor-k, end-start);
    const char *p = k;
    const char *q = start;
    for (; n && *p == *q; p++, q++, n--) { }
    if (match->length < p-k) {
      match->length = p-k;
      match->distance = w->cursor-k;
    }
    k++;
  }
  return 0;
}

const char *window_distance(window_t *w, int distance)
{
  const char *p = w->cursor-distance;
  return p < w->start? 0: p;
}

void window_append_match(window_t *w, const match_t *m)
{
  int n = m->length;
  while (n--) {
    window_append(w, *window_distance(w, m->distance));
  }
}
