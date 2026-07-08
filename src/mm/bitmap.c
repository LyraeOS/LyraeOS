#include <mm/bitmap.h>
uint64_t bitmap_check_bit(bitmap_t *bitmap, size_t bit) {
    return bitmap->map[bit / 64] & (1ULL << (bit % 64));
}

void bitmap_set_bit(bitmap_t *bitmap, size_t bit) {
    bitmap->map[bit / 64] |= (1ULL << (bit % 64));
}

void bitmap_unset_bit(bitmap_t *bitmap, size_t bit) {
    bitmap->map[bit / 64] &= ~(1 << (bit % 64));
}
