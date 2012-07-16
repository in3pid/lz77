#ifndef MATCH_H
#define MATCH_H

#include <stdint.h>

extern const int match_distance_max;
extern const int match_length_max;

typedef uint16_t match_t;

match_t match_pack(uint16_t distance, uint16_t length);
int match_distance(match_t m);
int match_length(match_t m);

#endif
