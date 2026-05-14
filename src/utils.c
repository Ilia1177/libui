#include "libui.h"

void *ui_realloc(void *ptr, size_t old_size, size_t new_size)
{
    if (!ptr)
        return malloc(new_size);
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }
    void *new_ptr = malloc(new_size);
    if (!new_ptr)
        return NULL;
    size_t copy_size = old_size < new_size ? old_size : new_size;
    memcpy(new_ptr, ptr, copy_size);
    free(ptr);
    return new_ptr;
}
