#include "window.h"

int min(int a, int b)
{
  return a < b? a: b;
}

int max(int a, int b)
{
  return a < b? b: a;
}


void window_init(window_t *window, int length)
{
  window->buffer = malloc(length);
  window->length = length;
  window->position = 0;
  bzero(window->buffer, window->length);
}

void window_free(window_t *window)
{
  free(window->buffer);
}

void window_append(window_t *window, char c)
{
    window->buffer[window->position++] = c;
    window_copyback(window);
}

void window_copyback(window_t *window)
{
  if (window->position < window->length) return;
  bcopy(window->buffer+1, window->buffer, window->length-1);
  window->position--;
}


int window_match(window_t *window, match_t *match, const char *data, int length)
{
  int i = 0;
  bzero(match, sizeof match);
  while (i < window->position) {
    int j = 0;
    int len = min(window->position, length);
    for (; data[j] == window->buffer[i] && len > 0; i++, j++, len--) { }
    if (j > match->length) {
      match->length = j;
      match->distance = window->position-(i-match->length);
    }
    if (j == 0) break;
  }
  return 0;
}
