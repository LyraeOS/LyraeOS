#ifndef LYRAEOS_BITMAP_H
#define LYRAEOS_BITMAP_H
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    uint64_t *map;
    size_t	 size;
} bitmap_t;

uint64_t bitmap_check_bit(bitmap_t *bitmap, size_t bit);
void bitmap_set_bit(bitmap_t *bitmap, size_t bit);
void bitmap_unset_bit(bitmap_t *bitmap, size_t bit);

#endif //LYRAEOS_BITMAP_H
