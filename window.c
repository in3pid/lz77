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

int window_match(window_t *w,
		 match_t *m,
		 const char *start,
		 const char *end)
{
  const char *a = w->cursor-w->block_size;
  const char *k = w->start < a? a: w->start;
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

const char *window_distance(window_t *w, int n)
{
  const char *p = w->cursor-n;
  return p < w->start? 0: p;
}

void window_append(window_t *w, char c)
{
    *w->cursor++ = c;
}

void window_append_match(window_t *w, const match_t *m)
{
  const char *p = window_distance(w, m->distance);
  int n = m->length;
  while (n--) *w->cursor++ = *p++;
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


uint16_t packed_match(const match_t *m)
{
  uint16_t d = m->distance & 1023;
  uint16_t e = m->length << 10;
  return d | e;
}

void unpack_match(match_t *m, uint16_t n)
{
  m->distance = n & 1023;
  m->length = n >> 10;
}
