#ifndef UI_GLOBAL_H
#define UI_GLOBAL_H
#include "libui.h"  // gets ui_pos_t fully defined, forward decl handles ui_win_t
#include "ui_box.h" // New: Include ui_box.h

#define APP_LOADING (1 << 0)
#define APP_QUIT    (1 << 1)

typedef struct ui_globalApp_s {
    SDL_Point mouse;
    ui_win_t* windows;
    ui_box_t* focused_box;
    uint32_t  state;
    void**    inputs;
    int       input_nb;
    void*     env;

    float scale_x;
    float scale_y;
    bool  loading;
    SDL_Color menu_color_1;
    SDL_Color menu_color_2;
    //
    // void (*on_key_down)(struct ui_globalApp_s*, SDL_Event*);
    // void (*on_key_up)(struct ui_globalApp_s*, SDL_Event*);
    // void (*on_mouse_button_down)(struct ui_globalApp_s*, SDL_Event*);
    // void (*on_mouse_button_up)(struct ui_globalApp_s*, SDL_Event*);
    // void (*on_mouse_motion)(struct ui_globalApp_s*, SDL_Event*);
    // void (*on_mouse_wheel)(struct ui_globalApp_s*, SDL_Event*);
    // void (*on_window_resized)(struct ui_globalApp_s*, ui_win_t*, int w, int h);
    //
    ui_tool_t*       active_tool;
    ui_boxhandler_t* tool;
    ui_winhandler_t* actions;
    void (*start)(struct ui_globalApp_s*);
} ui_globalApp_t;

void            ui_global_get_scale(ui_globalApp_t* app);
ui_globalApp_t* ui_global_init(char* name, void* env);
void            ui_global_free(ui_globalApp_t*);
void            ui_start(ui_globalApp_t* app);
void            ui_global_add_window(ui_globalApp_t* app, ui_win_t *win);
void            ui_global_remove_window(ui_globalApp_t* app, ui_win_t *win);

// // Event registration functions for ui_globalApp_t (now taking SDL_Event*)
// void ui_global_set_on_key_down(ui_globalApp_t* a, void(*handler)(ui_globalApp_t*, SDL_Event*));
// void ui_global_set_on_key_up(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_Event*));
// void ui_global_set_on_mouse_button_down(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*,
// SDL_Event*)); void ui_global_set_on_mouse_button_up(ui_globalApp_t* app,
// void(*handler)(ui_globalApp_t*, SDL_Event*)); void ui_global_set_on_mouse_motion(ui_globalApp_t*
// app, void(*handler)(ui_globalApp_t*, SDL_Event*)); void
// ui_global_set_on_mouse_wheel(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*, SDL_Event*));
// void ui_global_set_on_window_resized(ui_globalApp_t* app, void(*handler)(ui_globalApp_t*,
// ui_win_t*, int w, int h)); // Keep this signature

#endif
