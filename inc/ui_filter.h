#ifndef UI_FILTER_H
# define UI_FILTER_H

# include "libui.h"

typedef struct ui_filhandler_s {
    ui_fhook_fn_t          fn;
    void*                   data;
    struct ui_filhandler_s* next;
}	ui_filhandler_t;

// Hook filter
void ui_filter_clean(ui_filhandler_t** list);
void ui_filter_prepend(ui_filhandler_t** list, ui_fhook_fn_t fn, void* d);
void ui_filter_append(ui_filhandler_t** list, ui_fhook_fn_t fn, void* d);
void ui_filter_remove(ui_filhandler_t** list, ui_fhook_fn_t fn);
void ui_filter_fire(ui_filhandler_t*, ui_layer_t*, SDL_Event*, void*);
void ui_filter_replace(ui_filhandler_t* handler, ui_fhook_fn_t old, ui_fhook_fn_t new);
#endif
