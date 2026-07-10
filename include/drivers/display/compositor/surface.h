#ifndef LYRAEOS_SURFACE_H
#define LYRAEOS_SURFACE_H
// basically a small framebuffer
typedef struct {
    uint32_t* buf;
    int w, h;
    int stride;
} surface_t;

surface_t surface_new(int w, int h);
void surface_blit(surface_t* src, surface_t* dest, int x, int y);

#endif // LYRAEOS_SURFACE_H
