#include "libui.h"
#ifndef UI_LAYER_H
# define UI_LAYER_H
# define LAYER_STALE (1 << 0)

typedef struct ui_layhandler_s 
{
    ui_lhook_fn_t           fn;
    void*                   data;
    struct ui_layhandler_s* next;
} ui_layhandler_t;

typedef struct ui_layer_s {
    short              state;
    int                border;
    const char*        label;
    ui_filhandler_t*   filters;
    void*              filters_data;
    SDL_Texture*       texture;
    ui_box_t*          parent_box;
    SDL_Rect           area;
    double             angle;
    SDL_BlendMode      blend_mode;
	ui_layhandler_t*   destroy;
    struct ui_layer_s* next;
} ui_layer_t;

void	ui_layer_remove(ui_layer_t**list, ui_layer_t* layer);
ui_layer_t* ui_layer_selected(ui_layer_t* layers, SDL_Point* p);
void        ui_layer_swap(ui_layer_t** head, ui_layer_t* a, ui_layer_t* b);
SDL_bool    ui_layer_point_in_rotated(ui_layer_t* layer, SDL_Point* p);
Uint32      ui_layer_pixel_at(SDL_Renderer* renderer, SDL_Point* p);
ui_layer_t* ui_layer_make(ui_box_t* cnv, SDL_Texture* texture);
void        ui_layer_destroy_all(ui_layer_t** list);
int         ui_layer_count(ui_layer_t* layers);
void        ui_layer_add(ui_layer_t** layers, ui_layer_t* new);
void        ui_layer_center_all(ui_layer_t* layer, ui_box_t* box);
void        ui_layer_draw_at(ui_layer_t* layer, SDL_Point p);
SDL_Rect    ui_layer_zoomed_area(ui_layer_t* layer);
void 		ui_lhook_destroy_default(ui_layer_t* l, SDL_Event* e, void *d);

void ui_lhook_clean(ui_layhandler_t** list);
void ui_lhook_prepend(ui_layhandler_t** list, ui_lhook_fn_t fn);
void ui_lhook_append(ui_layhandler_t** list, ui_lhook_fn_t fn);
void ui_lhook_remove(ui_layhandler_t** list, ui_lhook_fn_t fn);
void ui_lhook_fire(ui_layhandler_t*, ui_layer_t*, SDL_Event*, void*);
void ui_lhook_replace(ui_layhandler_t* handler, ui_lhook_fn_t old, ui_lhook_fn_t);
#endif
