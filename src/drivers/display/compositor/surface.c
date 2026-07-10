#include <mm/kheap.h>
#include <mm/mem.h>
#include <stdint.h>
#include <stddef.h>
#include <drivers/display/compositor/surface.h>
#include <kernel/logging.h>

// will allocate memory, so free your buffer
surface_t surface_new(int w, int h) {
    surface_t new = {
        .w = w,
        .h = h,
        .stride = w,
    };
    LOG_DEBUG("attempting to allocate buffer");
    uint32_t* buf = (uint32_t* )kmalloc(w * h * sizeof(uint32_t));
    LOG_DEBUG("please work :(");
    new.buf = buf;
    return new;
}

void surface_blit(surface_t* src, surface_t* dest, int dest_x, int dest_y) {
    for (int y = 0; y < src->h; y++) {
        uint32_t *dest_ptr = dest->buf + (dest_y + y) * src->w + dest_x;
        uint32_t *src_ptr = src->buf + y * src->w;

        memcpy(dest_ptr, src_ptr, src->w * sizeof(uint32_t));
    }
}
