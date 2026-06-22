#ifndef LIBUI_H
#define LIBUI_H

#define PURPLE            (SDL_Color){118, 91, 167, 255}
#define ORANGE            (SDL_Color){255, 108, 47, 255}
#define TEAL              (SDL_Color){0, 131, 138, 255}
#define COLOR_BG          (SDL_Color){20, 20, 20, 255}
#define COLOR_WHITE       (SDL_Color){255, 255, 255, 255}
#define COLOR_TRANSPARENT (SDL_Color){0, 0, 0, 0}

#define UI_COLOR_BG   (SDL_Color){128, 128, 128, 255}
#define UI_COLOR_TEXT (SDL_Color){255, 255, 255, 255}
#define UI_COLOR_A    (SDL_Color){150, 150, 150, 255}
#define UI_COLOR_B    (SDL_Color){40, 40, 40, 255}

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// debug var
extern int box_nb;
extern int layer_nb;

// ui_global.h
typedef struct ui_globalApp_s ui_globalApp_t;

// ui_box.h
typedef struct ui_box_s       ui_box_t;
typedef struct ui_boxhandler_s ui_boxhandler_t;

// ui_win.h
typedef struct ui_win_s       ui_win_t;
typedef struct ui_winhandler_s ui_winhandler_t;

// ui_layer.h
typedef struct ui_layer_s     ui_layer_t;
typedef struct ui_layhandler_s ui_layhandler_t;

// ui_elem.h
typedef struct ui_slider_data_s ui_slider_data_t;

// ui_tool.h
typedef struct ui_tool_s ui_tool_t;

// ui_filter.h
typedef struct ui_filhandler_s ui_filhandler_t;

typedef int (*ui_whook_fn_t)(ui_win_t*, SDL_Event*, void*);
typedef void (*ui_bhook_fn_t)(ui_box_t*, SDL_Event*, void*);
typedef void (*ui_lhook_fn_t)(ui_layer_t*, SDL_Event*, void*); //duplicate signature
typedef void (*ui_fhook_fn_t)(ui_layer_t*, SDL_Event*, void*); //duplicate signature

typedef struct ui_scale_s {
    float x;
    float y;
} ui_scale_t;

// Event Hook -- Should be append ALLWAYS


// Destroy Hook -- Should be prepend ALLWAYS
// Update Hook	-- Shoulb pe prepend
// Render Hook	-- Should be reaplaced or append

ui_slider_data_t ui_slider_data(float max, float min, float def);
SDL_Texture*     create_white_texture(SDL_Renderer* renderer, int w, int h);
SDL_Point        ui_layer_mousepos(ui_layer_t* box);
SDL_Point        ui_box_mousepos(ui_box_t* box);
SDL_Point        ui_win_mousepos(ui_win_t* win);
int      ui_init();
int      ui_quit();
void     ui_log(const char* msg);
void     ui_log_eventstr(SDL_Event* e);
char*    ui_get_time(void);
void*    ui_realloc(void* ptr, size_t old_size, size_t new_size);
SDL_Rect ui_area_center(SDL_Rect from, SDL_Rect to_center);
SDL_Rect ui_area(int, int, int, int);
void     ui_reset_state_and_input(ui_globalApp_t* app, bool* running);
void        transfert_all_input(ui_globalApp_t* app, ui_box_t* box);

// Texture utils
SDL_Texture* ui_tex_path(SDL_Renderer* render, const char* path);
SDL_Texture* ui_tex_str(ui_win_t* win, const char* text, SDL_Color color);
float        clampf(float, float, float);

#include "ui_box.h"
#include "ui_elem.h"
#include "ui_global.h"
#include "ui_win.h"
#include "ui_layer.h"
#include "ui_filter.h"
#include "ui_layout.h"

#endif
