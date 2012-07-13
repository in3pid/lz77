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
  w->buffer = malloc(length);
  w->length = length;
  w->position = 0;
  memset(w->buffer, 0, w->length);
}

void window_free(window_t *w)
{
  free(w->buffer);
}

void window_append(window_t *w, char c)
{
    w->buffer[w->position++] = c;
    window_copyback(w);
}

void window_copyback(window_t *w)
{
  if (w->position < w->length) return;
  memmove(w->buffer, w->buffer+1, w->length-1);
  w->position--;
}


int window_match(window_t *w, match_t *match, const char *data, int length)
{
  int i = 0;
  memset(match, 0, sizeof match);
  while (i < w->position) {
    int j = 0;
    int len = min(w->position, length);
    for (; data[j] == w->buffer[i] && len > 0; i++, j++, len--) { }
    if (j > match->length) {
      match->length = j;
      match->distance = w->position-(i-match->length);
    }
    if (j == 0) break;
  }
  return 0;
}

const char *window_distance(window_t *w, int distance)
{
  const char *p = w->buffer + w->position - distance;
  return p < w->buffer? w->buffer: p;
}
