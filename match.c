#include "match.h"

const int match_distance_max = 1023;
const int match_length_max = 63;

static uint16_t min(uint16_t lhs, uint16_t rhs)
{
  return lhs < rhs? lhs: rhs;
}

match_t match_pack(uint16_t distance, uint16_t length)
{
  return min(distance, 1023) | (min(length, 63) << 10);
}

int match_distance(match_t m)
{
  return m & 1023;
}

int match_length(match_t m)
{
  return m >> 10;
}
