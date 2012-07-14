#include <stdlib.h>
#include <string.h>
#include "window.h"

static int min(int a, int b)
{
  return a < b? a: b;
}

static int max(int a, int b)
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

int window_match(window_t *w,
		 match_t *m,
		 const char *start,
		 const char *end)
{
  const char *k = w->start;
  memset(m, 0, sizeof *m);
  while (k < w->cursor) {
    int n = min(w->cursor-k, end-start);
    const char *p = k;
    const char *q = start;
    for (; n && *p == *q; p++, q++, n--) { }
    if (m->length < p-k) {
      m->length = p-k;
      m->distance = w->cursor-k;
    }
    k++;
  }
  return m->length;
}

void window_append(window_t *w, char c)
{
    *w->cursor++ = c;
    window_slide(w);
}

// TODO this takes a long time

void window_append_match(window_t *w, const match_t *m)
{
  int n = m->length;
  while (n--) {
    window_append(w, *window_distance(w, m->distance));
  }
}

const char *window_distance(window_t *w, int n)
{
  const char *p = w->cursor-n;
  return p < w->start? 0: p;
}

void window_slide(window_t *w)
{
  if (w->cursor < w->end) return;
  memmove(w->start, w->start+1, w->end-w->start);
  w->cursor--;
}
